#include "dither_shuffling.h"
#include <map>
#include <vector>
#include <algorithm>
#include <time.h>
#include <hash_map>
#include <map>
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

	// shuffling G & B
	unsigned int t;
	t = (x ^ (x >> 4)) & 0x000000F0; x = x ^ t ^ (t << 4);
	t = (x ^ (x >> 2)) & 0x00000C0C; x = x ^ t ^ (t << 2);
	t = (x ^ (x >> 1)) & 0x00002222; x = x ^ t ^ (t << 1);

	// shuffling R & G & B
	x = (x & 0x0000FF00) << 4 | (x >> 8) & 0x00000F00 | x & 0xFFF000FF;
	x = (x & 0x000F00F0) << 2 | (x >> 4) & 0x00030030 | x & 0xFFC0FC0F;
	x = (x & 0x0030C30C) << 1 | (x >> 2) & 0x00104104 | x & 0xFF8F38F3;
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

	// 能否不排序来解决?
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

