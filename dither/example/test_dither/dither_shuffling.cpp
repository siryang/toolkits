#include "dither_shuffling.h"
#include <vector>
#include <algorithm>
#include <time.h>
extern "C"
{
#include "cq_types.h"
#include "cq_stdlib.h"
};

#include "c_algorithm_dec.h"
#include "c_algorithm_def.h"

using namespace std;

inline unsigned int shuffling(unsigned int x)
{
	//ab cd ef => e->c->b->d->e  => ac eb df
	x = x & 0xFFF0000F	// A | F
		|x << 4 & 0x000F0000 | x << 8 & 0x0000F000 // C | E
		|x >> 8 & 0x00000F00 | x >> 4 & 0x000000F0; // B | D

	x = x & 0xFFC03C03
		| x << 2 & 0x00300300 | x << 4 & 0x000C00C0
		| x >> 4 & 0x00030030 | x >> 2 & 0x0000C00C;

	x = x & 0xFF861861 
		| x << 1 & 0x00410410 | x << 2 & 0x00208208
		| x >> 2 & 0x00104104 | x >> 1 & 0x00082082;

	return x;
}

struct PixelNode
{
	int key;
	PixelType color;
};

#define PixelNode_less(left, right) ((left)->key < (right)->key)

algorithm_declare(PixelNode);
algorithm_define(PixelNode)

inline bool lessPixelNodeKey(PixelNode left, PixelNode right)
{
	return left.key < right.key;
}

void midSort(PixelNode* start, PixelNode* end, int refKey)
{
	PixelNode* p = start;
	PixelNode* q = end - 1;

	while(q < p)
	{
		while(p->key <= refKey){
			p++;
		}

		if (q->key > )
	}
}

void makePalette(PixelNode* pixelStart, int pixelNumber, PixelType* palettes, int maxPaletteNum)
{
	PixelType* pattle = palettes;
	int unitNumber;
	int r = 0,g = 0,b = 0;
	PixelNode* pixelEnd = pixelStart + pixelNumber;
	PixelNode* pixel = pixelStart;
	for (int paletteIdx = 0; paletteIdx < maxPaletteNum; paletteIdx++)
	{
		if (pixelNumber == 0){
			maxPaletteNum = paletteIdx;
			break;
		}

		unitNumber = (paletteIdx == maxPaletteNum - 1)? pixelNumber
			: (pixelNumber / (maxPaletteNum - paletteIdx));
		
		const PixelNode* lp;
		int i = 0;
		for (; i < unitNumber; i++)
		{
			lp = pixel++;
			// contribution to pattle color
			r += Pixel_getR(lp->color);
			g += Pixel_getG(lp->color);
			b += Pixel_getB(lp->color);
		}

		if (paletteIdx != maxPaletteNum - 1)
		{
			for (; pixel != pixelEnd && lp->color == pixel->color; i++)
			{
				lp = pixel++;
				r += Pixel_getR(lp->color);
				g += Pixel_getG(lp->color);
				b += Pixel_getB(lp->color);
			}
		}

		r /= i; g /= i; b /= i;
		*pattle++ = Pixel_assemblePixel(r, g, b);
		pixelNumber -= i;
	}
}

int extractPalete_shuffling( PixelType* buffer, int width, int height, int pitchInPixel, PixelType* palettes, int maxPaletteNum )
{	
	clock_t start = clock();
	// 1.get freq
	int pixelNumber = width * height;
	vector<PixelNode> pixels;
	pixels.resize(pixelNumber);

	PixelNode* pixelStart = &pixels[0];
	PixelNode* pixelEnd = pixelStart + pixelNumber;
	PixelNode* pixel = pixelStart;

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			PixelNode& node = *pixel++;
			node.color = buffer[y * pitchInPixel + x];
			node.key = shuffling(node.color);
		}
	}
	clock_t copyEnd = clock();

	// 可以用部分排序优化
	// 2.sort color
	PixelNode_sort(pixelStart, pixelEnd);
	//sort(pixelStart, pixelEnd, lessPixelNodeKey);
	clock_t sortEnd = clock();

	// make pattle
	makePalette(pixelStart, pixelNumber, palettes, maxPaletteNum);

	clock_t getPattleEnd = clock();

#define GetFloatCost(start, end) ((end) - (start)) / 1000.0f
	printf("total cost : %3f, copy cost %3f, sort cost: %3f, palette cost: %3f\n",
		GetFloatCost(start, getPattleEnd),
		GetFloatCost(start, copyEnd),
		GetFloatCost(copyEnd, sortEnd),
		GetFloatCost(sortEnd, getPattleEnd));

	return maxPaletteNum;
}








