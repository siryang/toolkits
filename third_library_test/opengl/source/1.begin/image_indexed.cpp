#include "image_indexed.h"
#include "png.h"

typedef struct PngWriter
{
	char* pEnd;
	char* p;
	BOOL hasError;
} PngWriter;

static void _pngWriterCallback(png_structp png_ptr, png_bytep data, png_size_t length)
{
	PngWriter* writer = (PngWriter*)png_ptr->io_ptr;
	if (writer->p + length > writer->pEnd) 
		writer->hasError = TRUE;
	else
	{
		memcpy(writer->p, data, length);
		writer->p += length;
	}
}

size_t writePng(void* pngBuffer, int width, int height, int pitch, void* dest, size_t maxBufferSize)
{
	png_struct* png;
	png_info* info;
	png_color* pngPalette;
	png_byte** rows;
	PngWriter writer;
	writer.p = (char*)dest;
	writer.pEnd = writer.p + maxBufferSize;
	writer.hasError = FALSE;

	png = png_create_write_struct(PNG_LIBPNG_VER_STRING,0,0,0);
	info = png_create_info_struct(png);

	png_set_sig_bytes(png, 0);

	png_set_write_fn(png, (png_voidp)(&writer), _pngWriterCallback, 0);

	png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGBA,
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(png, info);

	rows = (png_byte**) malloc(height * sizeof(png_byte*));
	{
		int y;
		png_byte* row = (png_byte*)pngBuffer;
		for (y = 0; y < height; y++)
		{
			rows[y] = row + y * pitch;
		}
	}

	png_write_rows(png, rows, height);

	png_write_end(png,NULL);

	png_destroy_write_struct(&png, &info);

	return writer.p - (char*)pngBuffer;
}

BOOL savePng(const cqWCHAR* filename, void* pngBuffer, int width, int height, int pitch)
{
	FILE* fp;
	void* buffer;
	size_t bufferSize;
	size_t sizeWritten;

	fp = _wfopen(filename, L"wb");

	if (fp ==NULL)
		return FALSE;

	bufferSize = width * height + 1024;
	buffer = malloc(bufferSize);

	bufferSize = writePng(pngBuffer, width, height, pitch, buffer, bufferSize);

	sizeWritten = fwrite(buffer, 1, bufferSize, fp);

	free(buffer);

	fclose(fp);

	return sizeWritten == bufferSize;
}

void testPng()
{
#define WHITE 0xFFFFFFFF
#define BLACK 0
	char pngTest[200 * 200] = {0};
	for (int y = 0; y < 200; y++)
	{
		pngTest[100] = BLACK;
	}

	savePng(L"line.png", pngTest, 200, 200, 200);
}
