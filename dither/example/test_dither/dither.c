#include "stdafx.h"
#include "gui_skin.h"
#include "png.h"
#pragma warning(disable:4996)

#define SCREEN_WIDTH   1800
#define SCREEN_HEIGHT  500

GDI* g_gdi;

typedef struct ColorF
{
	unsigned int r;
	unsigned int g;
	unsigned int b;
	PixelType color;
	int16 freq;
	int16 isdelete;
}ColorF;

typedef struct PngMemory
{
	char* buffer;
	size_t len;
	size_t maxSize;
}PngMemory;

typedef struct CalPng
{
	BOOL isReady;

	uint16 maxWidth;
	uint16 maxHeight;

	char* outBuffer;
	int maxOutSize;
	
	png_byte** row_pointers;

	char* pixelBuffer;

	ColorF* cfs;
	uint16 maxCfNum;
	uint16 cfNum;

	PixelType rsvPixels[256];
	int rsvPixelNum;

	int unitPts[256];
	uint16 unitPtNum;
	uint16 maxUnitNum;
}CalPng;

static CalPng s_calPng;

void png_encoder_init()
{
	int i;
	
	s_calPng.maxWidth = 1500;
	s_calPng.maxHeight = 1500;

	s_calPng.row_pointers = (png_byte**)malloc(sizeof(png_byte*) * s_calPng.maxHeight);

	s_calPng.maxOutSize = s_calPng.maxWidth * s_calPng.maxHeight;
	s_calPng.outBuffer = (char*)malloc(sizeof(char) * s_calPng.maxOutSize);
	s_calPng.pixelBuffer = (char*)malloc(sizeof(char) * s_calPng.maxWidth * s_calPng.maxHeight);

	s_calPng.maxCfNum = s_calPng.maxWidth * s_calPng.maxHeight;
	s_calPng.cfs = (ColorF*)malloc(sizeof(ColorF) * s_calPng.maxCfNum);

	s_calPng.rsvPixelNum = 11;
	s_calPng.rsvPixels[0] = Pixel_assemblePixel(179,209,255);//water
	s_calPng.rsvPixels[1] = Pixel_assemblePixel(201,223,175);//green
	s_calPng.rsvPixels[2] = Pixel_assemblePixel(250,158,37);//most yellow road
	s_calPng.rsvPixels[3] = Pixel_assemblePixel(247,246,245);//background
	s_calPng.rsvPixels[4] = Pixel_assemblePixel(255,191,55);//second yellow road
	s_calPng.rsvPixels[5] = Pixel_assemblePixel(255,255,104);//third yellow road
	s_calPng.rsvPixels[6] = Pixel_assemblePixel(253,247,216);//fourth yellow road
	s_calPng.rsvPixels[7] = Pixel_assemblePixel(62,62,62);//black road
	s_calPng.rsvPixels[8] = Pixel_assemblePixel(98,131,51);//green road
	s_calPng.rsvPixels[9] = Pixel_assemblePixel(136,170,187);//gray label
	s_calPng.rsvPixels[10] = Pixel_assemblePixel(0,102,187);//subway label

	for (i = 0; i < s_calPng.rsvPixelNum; i++)
	{
		s_calPng.rsvPixels[i] |= 0xFF000000;
	}

	s_calPng.maxUnitNum = 256;

	s_calPng.isReady = TRUE;
}

void png_encoder_destruct()
{
	free(s_calPng.row_pointers);
	free(s_calPng.outBuffer);
	free(s_calPng.pixelBuffer);
	free(s_calPng.cfs);
	s_calPng.isReady = FALSE;
}

static int _compare_cf_r(const void* unit1, const void* unit2)
{
	return ((ColorF*)unit1)->r - ((ColorF*)unit2)->r;
}

static int _compare_cf_g(const void* unit1, const void* unit2)
{
	return ((ColorF*)unit1)->g - ((ColorF*)unit2)->g;
}

static int _compare_cf_b(const void* unit1, const void* unit2)
{
	return ((ColorF*)unit1)->b - ((ColorF*)unit2)->b;
}

static int _compare_color(const void* unit1, const void* unit2)
{
	return ((ColorF*)unit1)->color - ((ColorF*)unit2)->color;
}

static int _geLog2(int i) 
{ 
	int ret=0;
	if (i == 0) return 0;
	while (i) { i=i>>1; ++ret; } 
	return ret - 1; 
} 

static int _binarySearchColor(ColorF* buffer, int num, PixelType color)
{
	int high = num - 1;
	int low = 0;
	int mid;
	
	while(low <= high)
	{
		mid = (low + high) / 2;
		if (buffer[mid].color == color) return mid;
		else if (buffer[mid].color < color) low = mid + 1;
		else high = mid - 1;
	}

	return -1;
}

png_color getAveragePixel(ColorF* cfs, int num)
{
	int i;
	unsigned int sumR = 0, sumG = 0, sumB = 0;
	int allCfNum = 0;
	png_color p;

	for (i = 0; i < num; i++)
	{
		sumR += cfs[i].r * cfs[i].freq;
		sumG += cfs[i].g * cfs[i].freq;
		sumB += cfs[i].b * cfs[i].freq;
		allCfNum += cfs[i].freq;
	}

	p.red = sumR / allCfNum;
	p.green = sumG / allCfNum;
	p.blue = sumB / allCfNum;
	return p;
}

PixelType getAveragePixel2(ColorF* cfs, int num)
{
	int i;
	unsigned int sumR = 0, sumG = 0, sumB = 0;
	int allCfNum = 0;

	for (i = 0; i < num; i++)
	{
		sumR += cfs[i].r * cfs[i].freq;
		sumG += cfs[i].g * cfs[i].freq;
		sumB += cfs[i].b * cfs[i].freq;

		allCfNum += cfs[i].freq;
	}

	return Pixel_assemblePixel(sumR / allCfNum, sumG / allCfNum, sumB / allCfNum);
}

static void _addOpenAndSort(ColorF* cfs, uint16* cfNum, PixelType cf)
{
	ColorF* currNode = NULL;
	int32 firstMid = 0;
	int32 low = 0;
	int32 high = *cfNum - 1;
	int32 mid;
	BOOL isFind = FALSE;

	while (low <= high)
	{
		mid = (low + high) / 2;
		firstMid = mid;

		currNode = cfs+mid;

		if (currNode->color /*>*/< cf)
		{
			low = mid + 1;
			isFind = TRUE;
		}
		else if (currNode->color == cf)
		{
			isFind = FALSE;
			break;
		}
		else 
		{
			high = mid - 1;
			isFind = FALSE;
		}
	}

	if (isFind) firstMid++;

	if (firstMid == *cfNum)
	{ // Insert at last.
		cfs[*cfNum].color = cf;
		cfs[*cfNum].r = Pixel_getR(cf);
		cfs[*cfNum].g = Pixel_getG(cf);
		cfs[*cfNum].b = Pixel_getB(cf);
		cfs[*cfNum].isdelete = FALSE;
		cfs[*cfNum].freq = 1;
	}
	else
	{ // Insert at middle.
		for (low = *cfNum; low >= firstMid; low--)
			cfs[low] = cfs[low - 1];

		cfs[firstMid].color = cf;
		cfs[firstMid].r = Pixel_getR(cf);
		cfs[firstMid].g = Pixel_getG(cf);
		cfs[firstMid].b = Pixel_getB(cf);
		cfs[firstMid].isdelete = FALSE;
		cfs[firstMid].freq = 1;
	}

	(*cfNum)++;
}

int extractPalete(PixelType* buffer, int width, int height, int pitchInPixel, png_colorp palettes, int maxPaletteNum)
{
	int i, x, y;
	int resPalette = 0, cidx;
	int circleNum = _geLog2(maxPaletteNum);
		
	if (circleNum == 0) return 0;
	if ( ! s_calPng.isReady) return 0;

	s_calPng.cfNum = 0;
	s_calPng.unitPtNum = 0;

	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			PixelType oc = buffer[y * pitchInPixel + x];
			oc |= 0xFF000000;
			cidx = _binarySearchColor(s_calPng.cfs, s_calPng.cfNum, oc);
			if (cidx == -1)
			{
				_addOpenAndSort(s_calPng.cfs, &s_calPng.cfNum, oc);
				/*s_calPng.cfs[s_calPng.cfNum].r = Pixel_getR(oc);
				s_calPng.cfs[s_calPng.cfNum].g = Pixel_getG(oc);
				s_calPng.cfs[s_calPng.cfNum].b = Pixel_getB(oc);
				s_calPng.cfs[s_calPng.cfNum].color = oc;
				s_calPng.cfs[s_calPng.cfNum].freq = 1;
				s_calPng.cfNum++;*/

				//qsort(s_calPng.cfs, s_calPng.cfNum, sizeof(ColorF), _compare_color);
			}
			else s_calPng.cfs[cidx].freq++;
		}
	}

	//palettes = NULL;//todo

	if (s_calPng.cfNum <= maxPaletteNum)
	{
		for (i = 0; i < s_calPng.cfNum; i++)
		{
			palettes[resPalette].red = s_calPng.cfs[i].r;
			palettes[resPalette].green = s_calPng.cfs[i].g;
			palettes[resPalette].blue = s_calPng.cfs[i].b;
			resPalette++;
		}
		return resPalette;
	}

	for (i = 0; i < s_calPng.rsvPixelNum; i++)
	{
		cidx = _binarySearchColor(s_calPng.cfs, s_calPng.cfNum, s_calPng.rsvPixels[i]);
		if (cidx != -1)
		{
			s_calPng.cfs[cidx].isdelete = 1;
			palettes[resPalette].red = s_calPng.cfs[cidx].r;
			palettes[resPalette].green = s_calPng.cfs[cidx].g;
			palettes[resPalette].blue = s_calPng.cfs[cidx].b;
			resPalette++;
			if (resPalette == maxPaletteNum)
			{
				resPalette = 0;
				break;
			}
		}
	}

	if (resPalette != 0)
	{
		for (i = 0; i < s_calPng.cfNum; i++)
		{
			if (s_calPng.cfs[i].isdelete == 1)
			{
				memmove(s_calPng.cfs+i, s_calPng.cfs+i+1, sizeof(ColorF) * (s_calPng.cfNum-i-1));
				s_calPng.cfNum--;
				i--;
			}
		}
	}

	for (x = 0; x < circleNum; x++)
	{
		if (x == 0)
		{
			qsort(s_calPng.cfs, s_calPng.cfNum, sizeof(ColorF), _compare_cf_r);
			s_calPng.unitPts[s_calPng.unitPtNum] = 0;
			s_calPng.unitPtNum++;

			s_calPng.unitPts[s_calPng.unitPtNum] = s_calPng.cfNum / 2;
			s_calPng.unitPtNum++;

			s_calPng.unitPts[s_calPng.unitPtNum] = s_calPng.cfNum;
			s_calPng.unitPtNum++;
		}
		else
		{
			int cutNum;
			cidx = x % 3;

			for (i = 0; i < s_calPng.unitPtNum - 1; i++)
			{
				if (cidx == 0)//r
					qsort(s_calPng.cfs+s_calPng.unitPts[i], s_calPng.unitPts[i+1] - s_calPng.unitPts[i], sizeof(ColorF), _compare_cf_r);
				else if (cidx == 1)//g
					qsort(s_calPng.cfs+s_calPng.unitPts[i], s_calPng.unitPts[i+1] - s_calPng.unitPts[i],  sizeof(ColorF), _compare_cf_g);
				else if (cidx == 2)//b
					qsort(s_calPng.cfs+s_calPng.unitPts[i], s_calPng.unitPts[i+1] - s_calPng.unitPts[i],  sizeof(ColorF), _compare_cf_b);

				cutNum = (s_calPng.unitPts[i+1] - s_calPng.unitPts[i]) / 2;
				memmove(s_calPng.unitPts+i+2, s_calPng.unitPts+i+1, sizeof(int) * (s_calPng.unitPtNum - i - 1));
				s_calPng.unitPtNum++;
				if (s_calPng.unitPtNum == s_calPng.maxUnitNum) goto NEXT;
				s_calPng.unitPts[i+1] = s_calPng.unitPts[i] + cutNum;
				i++;
			}//for (i = 0; i < iniNum; i++)
		}
	}//for (x = 0; x < 4; x++)

NEXT:
	for (i = 0; i < s_calPng.unitPtNum-1; i++)
	{
		palettes[resPalette] =  getAveragePixel(s_calPng.cfs+s_calPng.unitPts[i], s_calPng.unitPts[i+1] - s_calPng.unitPts[i]);
		resPalette++;
		if (resPalette == maxPaletteNum) break;
	}

	return resPalette;
}

int extractPalete2(PixelType* buffer, int width, int height, int pitchInPixel, PixelType* palettes, int maxPaletteNum, PixelType* rsvPixels, int rsvPixelNum)
{
	int i, x, y;
	ColorF* cfs = (ColorF*)malloc(sizeof(ColorF) * (width * height));
	int cfNum = 0, resPalette = 0, cidx;
	int circleNum = _geLog2(maxPaletteNum);
	int unitPts[256];
	int unitPtNum = 0;

	if (circleNum == 0) return 0;

	cq_memset(cfs, 0, sizeof(ColorF) * (width * height));

	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			PixelType oc = buffer[y * pitchInPixel + x];
			oc |= 0xFF000000;
			cidx = _binarySearchColor(cfs, cfNum, oc);
			if (cidx == -1)
			{
				cfs[cfNum].color = oc;
				cfs[cfNum].freq = 1;
				cfNum++;

				qsort(cfs, cfNum, sizeof(ColorF), _compare_color);
			}
			else cfs[cidx].freq++;
		}
	}

	if (cfNum <= maxPaletteNum)
	{
		for (i = 0; i < cfNum; i++)
		{
			palettes[resPalette] = cfs[i].color;
			resPalette++;
		}
		free(cfs);
		return resPalette;
	}

	for (i = 0; i < rsvPixelNum; i++)
	{
		rsvPixels[i] |= 0xFF000000;
		cidx = _binarySearchColor(cfs, cfNum, rsvPixels[i]);
		if (cidx != -1)
		{
			palettes[resPalette] =  cfs[cidx].color;
			resPalette++;
			if (resPalette == maxPaletteNum) return resPalette;

			memmove(cfs+cidx, cfs+cidx+1, sizeof(ColorF) * (cfNum-cidx-1));
			cfNum--;
			i--;
		}
	}

	for (i = 0; i < cfNum; i++)
	{
		cfs[i].r = Pixel_getR(cfs[i].color);
		cfs[i].g = Pixel_getG(cfs[i].color);
		cfs[i].b = Pixel_getB(cfs[i].color);
	}

	for (x = 0; x < circleNum; x++)
	{
		if (x == 0)
		{
			qsort(cfs, cfNum, sizeof(ColorF), _compare_cf_r);
			unitPts[unitPtNum] = 0;
			unitPtNum++;

			unitPts[unitPtNum] = cfNum / 2;
			unitPtNum++;

			unitPts[unitPtNum] = cfNum;
			unitPtNum++;
		}
		else
		{
			int cutNum;
			cidx = x % 3;

			for (i = 0; i < unitPtNum - 1; i++)
			{
				if (cidx == 0)//r
					qsort(cfs+unitPts[i], unitPts[i+1] - unitPts[i], sizeof(ColorF), _compare_cf_r);
				else if (cidx == 1)//g
					qsort(cfs+unitPts[i], unitPts[i+1] - unitPts[i],  sizeof(ColorF), _compare_cf_g);
				else if (cidx == 2)//b
					qsort(cfs+unitPts[i], unitPts[i+1] - unitPts[i],  sizeof(ColorF), _compare_cf_b);

				cutNum = (unitPts[i+1] - unitPts[i]) / 2;
				memmove(unitPts+i+2, unitPts+i+1, sizeof(int) * (unitPtNum - i - 1));
				unitPtNum++;
				if (unitPtNum == 256) goto NEXT;
				unitPts[i+1] = unitPts[i] + cutNum;
				i++;
			}//for (i = 0; i < iniNum; i++)
		}
	}//for (x = 0; x < 4; x++)

NEXT:
	for (i = 0; i < unitPtNum-1; i++)
	{
		palettes[resPalette] =  getAveragePixel2(cfs+unitPts[i], unitPts[i+1] - unitPts[i]);
		resPalette++;
		if (resPalette == maxPaletteNum) break;
	}

	free(cfs);
	return resPalette;
}

unsigned int matchPaletteToIndex(PixelType c, const PixelType* palette, size_t num)
{
	size_t i;
	unsigned int bestIndex = 0;
	int bestScore = INT_MAX;
	for(i = 0; i < num; i++)
	{
		PixelType c2 = palette[i];
		int r = Pixel_getR(c), g = Pixel_getG(c), b = Pixel_getB(c);
		int r2 = Pixel_getR(c2), g2 = Pixel_getG(c2), b2 = Pixel_getB(c2);		

		int distance = cq_abs(r - r2) + cq_abs(g - g2) + cq_abs(b - b2);
		if (distance < bestScore)
		{
			bestScore = distance;
			bestIndex = i;
		}
	}

	return bestIndex;
}

PixelType matchPalette(PixelType c, const PixelType* palette, size_t num)
{
	return palette[matchPaletteToIndex(c, palette, num)];
}

unsigned int matchPaletteToIndex2(PixelType c, const png_color* palette, size_t num)
{
	size_t i;
	unsigned int bestIndex = 0;
	int bestScore = INT_MAX;
	for(i = 0; i < num; i++)
	{
		png_color c2 = palette[i];
		int r = Pixel_getR(c), g = Pixel_getG(c), b = Pixel_getB(c);
		int r2 = c2.red, g2 = c2.green, b2 = c2.blue;

		int distance = cq_abs(r - r2) + cq_abs(g - g2) + cq_abs(b - b2);
		if (distance < bestScore)
		{
			bestScore = distance;
			bestIndex = i;
		}
	}

	return bestIndex;
}

void applyError(PixelType* buffer, int pitchInPixel, int x, int y, int errR, int errG, int errB, int factor)
{
	PixelType c = buffer[y * pitchInPixel + x];
	int r = Pixel_getR(c);
	int g = Pixel_getG(c);
	int b = Pixel_getB(c);

	r = max(0, min(Pixel_channelMaxR, r + errR * factor / 16));
	g = max(0, min(Pixel_channelMaxG, g + errG * factor / 16));
	b = max(0, min(Pixel_channelMaxB, b + errB * factor / 16));

	buffer[y * pitchInPixel + x] = Pixel_assemblePixel4(r, g, b, Pixel_channelMaxA);
}

png_byte** Png_getRowPointers(PixelType* pixels, int w, int h, int pitch, png_color* palette, int paletteNum)
{
	png_byte** dest = s_calPng.row_pointers;
	char *p,  *pEnd;
	int y, x;
	
	for(y = 0; y < h; y++)
	{
		for(x = 0; x < w; x++)
		{
			s_calPng.pixelBuffer[y * w + x] = (unsigned char)matchPaletteToIndex2(pixels[y * pitch + x], palette, paletteNum);
		}
	}
	p = s_calPng.pixelBuffer;
	pEnd = p + h * w;
	for( ; p < pEnd; p += w)
	{
		*dest++ = p;
	}

	return s_calPng.row_pointers;
}

void pngEncoder_write_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
	PngMemory* pngMemory = (PngMemory*)png_ptr->io_ptr;
	if (pngMemory->len + length > pngMemory->maxSize) return;
	memcpy(pngMemory->buffer + pngMemory->len, data, length);
	pngMemory->len += length;
}

static BOOL PngEncoder_construct(png_structp* png_ptr, png_infop* info_ptr)
{
	*png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,0,0,0);
	if (*png_ptr == NULL) return FALSE;

	*info_ptr = png_create_info_struct(*png_ptr);
	if (*info_ptr == NULL) return FALSE;

	// set error handling
	if (setjmp(png_jmpbuf(*png_ptr)))
	{
		png_destroy_write_struct(png_ptr, info_ptr);
		return FALSE;
	} 

	return TRUE;
}

static void _PngEncoder_write(png_struct* png_ptr, png_info* info_ptr, png_bytep* row_pointers)
{
	png_write_info(png_ptr, info_ptr);

	png_write_image(png_ptr, row_pointers);

	png_write_end(png_ptr,NULL);
}

static void _PngEncoder_destruct(png_struct* png_ptr, png_info* info_ptr)
{
	png_destroy_write_struct(&png_ptr, &info_ptr);
}

size_t PngEncoder(void* buffer, int w, int h, int pitch, void** outputBuffer)
{
	png_struct*	png_ptr;
	png_info*	info_ptr;
	png_bytep* row_pointers = NULL;
	png_colorp palette = NULL;
	int y = 0, x = 0;
	int paletteNum;
	PixelType* pixels = (PixelType*)buffer;
	PngMemory pngMemory;

	*outputBuffer = s_calPng.outBuffer;
	pngMemory.buffer = *outputBuffer;
	pngMemory.len = 0;
	pngMemory.maxSize = s_calPng.maxOutSize;
	
	if (pixels == NULL || *outputBuffer == NULL) return 0;
	if (w == 0 || h == 0 || s_calPng.maxOutSize == 0) return 0;

	if ( ! PngEncoder_construct(&png_ptr, &info_ptr)) return 0;
	
	png_set_sig_bytes(png_ptr, 0);
	png_set_write_fn(png_ptr, (png_voidp)(&pngMemory),  pngEncoder_write_data, 0);
	
	palette = (png_colorp)png_malloc(png_ptr, 32 * png_sizeof(png_color));
	//paletteNum = extractPalete(pixels, w, h, pitch, palette, 32);

	{
		clock_t begin = clock();
		int i;
		float timeUsed;
		for (i = 0; i < 100; i++)
			paletteNum = extractPalete(pixels, w, h, pitch, palette, 32);
		timeUsed = (clock()- begin) / 1000.0f;
		printf("extract palette time: %f", timeUsed);
	}
	
	row_pointers = Png_getRowPointers(pixels, w, h, pitch, palette, paletteNum);

	png_set_IHDR(png_ptr, info_ptr, w, h, 8, PNG_COLOR_TYPE_PALETTE,
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_set_PLTE(png_ptr, info_ptr, palette, paletteNum);

	_PngEncoder_write(png_ptr, info_ptr, row_pointers);

	if (palette != NULL)
	{
		png_free(png_ptr, palette);
		palette = NULL;
	}

	_PngEncoder_destruct(png_ptr, info_ptr);

	return pngMemory.len;
}

// http://en.wikipedia.org/wiki/Floyd%E2%80%93Steinberg_dithering
void process2(PixelType* buffer, int width, int height, int pitchInPixel)
{
	FILE* file;
	int bufferSize;
	PixelType* outBuffer;

	width = 300;
	height = 300;
	
	bufferSize = PngEncoder(buffer, width, height, pitchInPixel, &outBuffer);
	file = _wfopen(L"write_png.png", L"wb");
	fwrite(outBuffer,  bufferSize, sizeof(char), file);
	fclose(file);
}

void process(PixelType* buffer, int width, int height, int pitchInPixel, BOOL dither)
{
	//PixelType palette[] = {Pixel_fromX8R8G8B8(0), Pixel_assemblePixel4(250, 79, 40, 255), Pixel_assemblePixel4(221, 30, 22, 255)};
	int maxPaletteNum = 32;
	PixelType* palette = (PixelType*)malloc(sizeof(PixelType) * maxPaletteNum);
	int paletteNum;
	int x, y;	
	int rsvPixelNum = 11;
	PixelType rsvPixels[256];

	rsvPixels[0] = Pixel_assemblePixel(179,209,255);//water
	rsvPixels[1] = Pixel_assemblePixel(201,223,175);//green
	rsvPixels[2] = Pixel_assemblePixel(250,158,37);//most yellow road
	rsvPixels[3] = Pixel_assemblePixel(247,246,245);//background
	rsvPixels[4] = Pixel_assemblePixel(255,191,55);//second yellow road
	rsvPixels[5] = Pixel_assemblePixel(255,255,104);//third yellow road
	rsvPixels[6] = Pixel_assemblePixel(253,247,216);//fourth yellow road
	rsvPixels[7] = Pixel_assemblePixel(62,62,62);//black road
	rsvPixels[8] = Pixel_assemblePixel(98,131,51);//green road
	rsvPixels[9] = Pixel_assemblePixel(136,170,187);//gray label
	rsvPixels[10] = Pixel_assemblePixel(0,102,187);//subway label

	paletteNum = extractPalete2(buffer, width, height, pitchInPixel, palette, maxPaletteNum, rsvPixels, rsvPixelNum);
	if (paletteNum == 0) return;

	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			PixelType oc = buffer[y * pitchInPixel + x];
			int or = Pixel_getR(oc), og = Pixel_getG(oc), ob = Pixel_getB(oc);

			PixelType nc = matchPalette(oc, palette, paletteNum);
			if (dither)
			{
				int nr = Pixel_getR(nc), ng = Pixel_getG(nc), nb = Pixel_getB(nc);

				int errR = (int)or - (int)nr;
				int errG = (int)og - (int)ng;
				int errB = (int)ob - (int)nb;

				if (x < width - 1)
					applyError(buffer, pitchInPixel, x + 1, y, errR, errG, errB, 7);
				if (x > 1 && y < height - 1)
					applyError(buffer, pitchInPixel, x - 1, y + 1, errR, errG, errB, 3);
				if (y < height - 1)	
					applyError(buffer, pitchInPixel, x, y + 1, errR, errG, errB, 5);
				if (x < width - 1 && y < height - 1)
					applyError(buffer, pitchInPixel, x + 1, y + 1, errR, errG, errB, 1);
			}

			buffer[y * pitchInPixel + x] = nc;
		}
	}

	free(palette);
}

static void render()
{
	void* p;
	
	p = App_lockFrameBuffer();
	if (p)
	{
		Handle surf;
		int screenWidth = App_getScreenWidth();
		int screenHeight = App_getScreenHeight();

		png_encoder_init();

		surf = Surface_alloc();
		Surface_createExternalBuffer(surf, p, 
			App_getScreenPitch() / sizeof(PixelType), 
			screenWidth, screenHeight);
		GDI_setRenderTarget(g_gdi, surf);
		//GDI_clear(g_gdi, NULL, Pixel_fromX8R8G8B8(0xffffffff));
		
		// render code here
		{
			PixelType* buffer;
			Handle image = Surface_alloc();
			Rect srcArea;
			Surface_loadExt(image, L"baboon.png"); 
			Surface_getArea(image, &srcArea);

			// draw original image.
			GDI_bitBlt(g_gdi, 0, 0, image, &srcArea);

			buffer = (PixelType*)Surface_lock(image);
			process2(buffer, Rect_getWidth(srcArea), Rect_getHeight(srcArea), Rect_getWidth(srcArea));//

			process(buffer, Rect_getWidth(srcArea), Rect_getHeight(srcArea), Rect_getWidth(srcArea), FALSE);

			GDI_bitBlt(g_gdi, srcArea.right + 5, 0, image, &srcArea);

			Surface_unlock(image);

			//Surface_loadExt(image,/*L"heart.png"*//*L"1_9.png"*/L"map4.png"); 
			//buffer = (PixelType*)Surface_lock(image);

			//process(buffer, Rect_getWidth(srcArea), Rect_getHeight(srcArea), Rect_getWidth(srcArea), TRUE);
			//GDI_bitBlt(g_gdi, 2 * (srcArea.right + 5), 0, image, &srcArea);

			Surface_unlock(image);

			Surface_destruct(image);
		}

		Surface_destruct(surf);
		App_unlockAndPresent(NULL);

		png_encoder_destruct();
	}
}

static void proc(AppEventType event, const void* params)
{
	UNUSED_VAR(params);
	switch(event)
	{
	case AppEvent_Init:
		Graphics_init();
		g_gdi = GDI_alloc(SCREEN_WIDTH, SCREEN_HEIGHT);

		render();
		break;
	case AppEvent_Cleanup:
		GDI_free(g_gdi);
		Graphics_cleanup();
		break;
	case AppEvent_Render:
		break;
	}
}

int Mapbar_main()
{
	Mapbar_Win32_enableOpenGL(FALSE);
	return App_run(SCREEN_WIDTH, SCREEN_HEIGHT, &proc);
}

