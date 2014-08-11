#include "dither_shuffling.h"
#include <map>
#include <vector>
#include <algorithm>
#include <time.h>
#include <hash_map>
#include <map>
#include "cq_types.h"

using namespace std;
using namespace stdext;

inline unsigned int shuffling(unsigned int x)
{
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
	int freq;
	int key;
	PixelType color;
};

inline bool lessPixelNodeKey(PixelNode left, PixelNode right)
{
	return left.key < right.key;
}

void makePalette(PixelNode* pixelStart, int pixelNumber, PixelType* palettes, int maxPaletteNum)
{
	PixelType* pattle = palettes;
	int unitNumber = pixelNumber / maxPaletteNum;
	int r = 0,g = 0,b = 0;
	PixelNode* pixelEnd = pixelStart + pixelNumber;
	PixelNode* pixel = pixelStart;
	for (int paletteIdx = 0; paletteIdx < maxPaletteNum; paletteIdx++)
	{
		if (paletteIdx == maxPaletteNum - 1)
		{
			unitNumber += pixelNumber % maxPaletteNum;
		}

		const PixelNode* lp;
		int totalFreq = 0;
		for (int i = 0; i < unitNumber; i++)
		{
			lp = pixel++;
			// contribution to pattle color
			r += Pixel_getR(lp->color) * lp->freq;
			g += Pixel_getG(lp->color) * lp->freq;
			b += Pixel_getB(lp->color) * lp->freq;
			totalFreq += lp->freq;
		}

		r /= totalFreq; g /= totalFreq; b /= totalFreq;
		*pattle++ = Pixel_assemblePixel(r, g, b);
	}
}

int extractPalete_shuffling( PixelType* buffer, int width, int height, int pitchInPixel, PixelType* palettes, int maxPaletteNum )
{	
	hash_map<PixelType, PixelNode*> colorMap;
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
			const PixelType& color = buffer[y * pitchInPixel + x];
			hash_map<PixelType, PixelNode*>::iterator iterNode = colorMap.find(color);
			if (iterNode == colorMap.end())
			{
				colorMap.insert(pair<PixelType, PixelNode*>(color, pixel));
				PixelNode& node = *pixel++;
				node.color = color;
				node.key = shuffling(color);
				node.freq = 1;
			}
			else
			{
				iterNode->second->freq++;
			}
		}
	}
	pixelEnd = pixel;
	clock_t copyEnd = clock();

	// 能否不排序来解决?
	// 2.sort color
	sort(pixelStart, pixelEnd, lessPixelNodeKey);
	clock_t sortEnd = clock();

	// make pattle
	makePalette(pixelStart, pixelEnd - pixelStart, palettes, maxPaletteNum);

	clock_t getPattleEnd = clock();

#define GetFloatCost(start, end) ((end) - (start)) / 1000.0f
	printf("total cost : %3f, copy cost %3f, sort cost: %3f, palette cost: %3f\n",
		GetFloatCost(start, getPattleEnd),
		GetFloatCost(start, copyEnd),
		GetFloatCost(copyEnd, sortEnd),
		GetFloatCost(sortEnd, getPattleEnd));

	return maxPaletteNum;
}

