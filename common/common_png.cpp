#include "common_png.h"
#include "png.h"


static inline const char* pixel_at (const char* bitmap, size_t pixelSize, int width, int x, int y)
{
	return bitmap + (width * y + x) * pixelSize;
}

bool writePng(const char* bitmap, size_t pixelSize, int width, int height, const char *path)
{
	FILE * fp;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	png_byte ** row_pointers = NULL;

	/* "status" contains the return value of this function. At first
	it is set to a value which means 'failure'. When the routine
	has finished its work, it is set to a value which means
	'success'. */
	bool status = false;
	/* The following number is set by trial and error only. I cannot
	see where it it is documented in the libpng manual.
	*/
	int depth = 8;

	fp = fopen (path, "wb");
	if (! fp) {
		goto fopen_failed;
	}

	png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		goto png_create_write_struct_failed;
	}

	info_ptr = png_create_info_struct (png_ptr);
	if (info_ptr == NULL) {
		goto png_create_info_struct_failed;
	}

	/* Set up error handling. */

	if (setjmp (png_jmpbuf (png_ptr))) {
		goto png_failure;
	}

	/* Set image attributes. */

	png_set_IHDR (png_ptr,
		info_ptr,
		width,
		height,
		depth,
		PNG_COLOR_TYPE_RGBA,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);

	/* Initialize rows of PNG. */

	row_pointers = (png_byte**)png_malloc (png_ptr, height * sizeof (png_byte *));
	for (int y = 0; y < height; ++y) 
	{
		png_byte *row = (png_byte*)png_malloc (png_ptr, width * pixelSize);
		row_pointers[y] = row;
		for (int x = 0; x < width; ++x) 
		{
			const char* pixel = pixel_at (bitmap, pixelSize, width, x, y);
			memcpy(row, pixel, pixelSize);
			row += pixelSize;
		}
	}

	/* Write the image data to "fp". */

	png_init_io (png_ptr, fp);
	png_set_rows (png_ptr, info_ptr, row_pointers);
	png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

	/* The routine has successfully written the file, so we set
	"status" to a value which indicates success. */

	status = true;

	for (int y = 0; y < height; y++) 
	{
		png_free (png_ptr, row_pointers[y]);
	}
	png_free (png_ptr, row_pointers);

png_failure:
png_create_info_struct_failed:
	png_destroy_write_struct (&png_ptr, &info_ptr);
png_create_write_struct_failed:
	fclose (fp);
fopen_failed:
	return status;
}

bool readPng( const wchar_t* fname, int* width, int* height, std::vector<char>& pngBuffer)
{
	FILE* fp = _wfopen(fname, L"rb");

	char header[8]; // header of png
	fread(header, 1, 8, fp);
	if (png_sig_cmp((png_const_bytep)header, 0, 8))
	{
		return false;
	}

	int depth, colorType;
	png_structp pngReader = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop pngInfo = png_create_info_struct (pngReader);

	setjmp(png_jmpbuf(pngReader));
	png_init_io (pngReader, fp);
	png_set_sig_bytes(pngReader, 8);

	png_read_info(pngReader, pngInfo);
	
	png_get_IHDR(pngReader, pngInfo, (png_uint_32*)width, (png_uint_32*)height, &depth, &colorType, NULL, NULL, NULL);
	/* read file */
	setjmp(png_jmpbuf(pngReader));
	png_bytep* rowPointers = (png_bytep*) malloc(sizeof(png_bytep) * *height);
	size_t rowLen = png_get_rowbytes(pngReader, pngInfo);
	pngBuffer.resize(rowLen * *height);
	for (int y=0; y<*height; y++)
	{
		rowPointers[y] = (png_bytep)&pngBuffer[y * rowLen];
	}

	png_read_image(pngReader, rowPointers);

	//unsigned char* data = (unsigned char*)png_get_mem_ptr(pngReader);
	//png_get_IHDR(pngReader, pngInfo, width, height, &depth, &colorType, NULL, NULL, NULL);
	//pngBuffer.resize((*width) * (*height) * 4);
	//memcpy(&pngBuffer[0], data, pngBuffer.size());
	free(rowPointers);
	png_destroy_info_struct(pngReader, &pngInfo);
	png_destroy_read_struct(&pngReader, &pngInfo, NULL);
	fclose(fp);
	return true;
}