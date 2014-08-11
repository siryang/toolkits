extern "C"
{
#include "stdafx.h"
#include "gui_skin.h"
#include "png.h"
};
#include <map>
#include <vector>
#include <algorithm>
#include "dither_octree.h"
#include "dither_shuffling.h"

using namespace std;

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
		*dest++ = (png_byte*)p;
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

void process(PixelType* buffer, int width, int height, int pitchInPixel, BOOL dither)
{
	//PixelType palette[] = {Pixel_fromX8R8G8B8(0), Pixel_assemblePixel4(250, 79, 40, 255), Pixel_assemblePixel4(221, 30, 22, 255)};
	int maxPaletteNum = 32;
	PixelType* palette = (PixelType*)malloc(sizeof(PixelType) * maxPaletteNum);
	int paletteNum;
	int x, y;	
	
	paletteNum = extractPalete_shuffling(buffer, width, height, pitchInPixel, palette, maxPaletteNum);
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
		Surface_createExternalBuffer(surf, (PixelType*)p, 
			App_getScreenPitch() / sizeof(PixelType), 
			screenWidth, screenHeight);
		GDI_setRenderTarget(g_gdi, surf);
		//GDI_clear(g_gdi, NULL, Pixel_fromX8R8G8B8(0xffffffff));
		
		// render code here
		{
			PixelType* buffer;
			Handle image = Surface_alloc();
			Rect srcArea;
			//Surface_loadExt(image, L"map4.png"); 
			//Surface_loadExt(image, L"lena.png"); 
			Surface_loadExt(image, L"baboon.png"); 
			Surface_getArea(image, &srcArea);

			// draw original image.
			GDI_bitBlt(g_gdi, 0, 0, image, &srcArea);

			buffer = (PixelType*)Surface_lock(image);

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

extern "C"
int Mapbar_main()
{
	Mapbar_Win32_enableOpenGL(FALSE);
	return App_run(SCREEN_WIDTH, SCREEN_HEIGHT, &proc);
}

