#include "dither_octree.h"
#include <map>
#include <vector>
#include <algorithm>
#include <time.h>

using namespace std;

const int g_maxRsvLevel = 6;

class ColorNode
{
public:
	int m_leafNum;
	PixelType  m_color;
	vector<ColorNode> m_childs;

	ColorNode* addColor(PixelType color);

	ColorNode* findNode(PixelType color)
	{
		ColorNode* begin = &m_childs[0];
		ColorNode* end = begin + m_childs.size();
		for (; begin < end; begin++)
		{
			if (begin->m_color == color){
				return begin;
			}
		}
		return NULL;


		//if (m_childs.empty())
		//	return NULL;

		//ColorNode* begin = &m_childs[0];
		//ColorNode* end = begin + m_childs.size();
		//
		//while (begin < end)
		//{
		//	ColorNode* mid = begin + (end - begin) / 2;
		//	PixelType midColor = mid->m_color;
		//	if (midColor == color){
		//		return mid;
		//	}
		//	else if (mid->m_color < color){
		//		end = mid;
		//	}
		//	else{
		//		begin = mid + 1;
		//	}
		//}
		
		return NULL;
	}
};


inline bool lessByNodeColor(const ColorNode& left, const ColorNode& right)
{
	return left.m_color < right.m_color;
}

ColorNode* ColorNode::addColor( PixelType color )
{
	ColorNode* node = findNode(color);
	if (node == NULL)
	{
		m_childs.push_back(ColorNode());
		node = &m_childs.back();
		node->m_color = color;
		node->m_leafNum = 0;
		//if(m_childs.size() > 1)
		//{
		//	sort(m_childs.begin(), m_childs.end(), lessByNodeColor);
		//}
	}
	return node;
}

PixelType getColorKey(PixelType color, int level)
{
	const static PixelType colorKeyMask[] = {
		0xFF808080, 0XFFC0C0C0, 0XFFE0E0E0, 0XFFF0F0F0,
		0XFFF8F8F8, 0XFFFCFCFC, 0XFFFEFEFE, 0XFFFFFFFF
	};
	return (PixelType)(color & colorKeyMask[level]);
}

void insertColor(ColorNode* root, PixelType color)
{
	vector<ColorNode*> traveledNodes;
	for (int i = 0; i < g_maxRsvLevel; i++)
	{
		PixelType key = getColorKey(color, i);
		if (i == g_maxRsvLevel - 1)
		{
			// is new leaf?
			ColorNode* leaf = root->findNode(color);
			if (leaf == NULL)
			{
				root->m_childs.push_back(ColorNode());
				leaf = &root->m_childs.back();
				leaf->m_color = color;
				//leaf->m_leafNum = 0;

				ColorNode** travleNode = &traveledNodes[0];
				ColorNode** travleNodeEnd = travleNode + traveledNodes.size();
				for (; travleNode != travleNodeEnd; travleNode++)
				{
					(*travleNode)->m_leafNum++;
				}
			}
		}
		else
		{
			root = root->addColor(key);
			traveledNodes.push_back(root);
		}
	}
}

int findNodeWithMaxFreq(const vector<ColorNode*>& nodes)
{
	int resultNodeIdx = 0;
	ColorNode* const* p = &nodes[0];
	int maxLeafNum = p[0]->m_leafNum;
	size_t len = nodes.size();
	for (size_t i = 1; i < len; i++)
	{
		if (maxLeafNum < p[i]->m_leafNum)
		{
			maxLeafNum = p[i]->m_leafNum;
			resultNodeIdx = i;
		}
	}
	return resultNodeIdx;
}

bool ColorNode_lessByLeafNum(ColorNode* left, ColorNode* right)
{
	return left->m_leafNum < right->m_leafNum;
}

int extractPalete_octree(PixelType* buffer, int width, int height, int pitchInPixel, PixelType* palettes, int maxPaletteNum)
{
	clock_t start = clock();
	int pixelNumber = width * height;
	vector<PixelType> pixels;
	pixels.resize(pixelNumber);

	PixelType* pixelStart = &pixels[0];
	PixelType* pixelEnd = pixelStart + pixelNumber;
	PixelType* pixel = pixelStart;

	ColorNode root;
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			*pixel++ = buffer[y * pitchInPixel + x];
		}
	}

	sort(pixelStart, pixelEnd);
	pixelEnd = unique(pixelStart, pixelEnd);

	clock_t sortEnd = clock();

	for (pixel = pixelStart; pixel < pixelEnd; pixel++)
	{
		insertColor(&root, *pixel);
	}

	clock_t insertEnd = clock();

	vector<ColorNode*> palettesNode;
	vector<ColorNode*> maxLeafNodes;
	maxLeafNodes.push_back(&root);
	while (!maxLeafNodes.empty())
	{
		if (maxLeafNodes.size() + palettesNode.size() == maxPaletteNum)
		{
			palettesNode.insert(palettesNode.end(), maxLeafNodes.begin(), maxLeafNodes.end());
			break;
		}

		// split max freq
		ColorNode* node = maxLeafNodes.front();
		pop_heap(maxLeafNodes.begin(), maxLeafNodes.end());
		maxLeafNodes.pop_back();

		if ((int)(node->m_childs.size() + maxLeafNodes.size() + palettesNode.size()) <= maxPaletteNum)
		{
			for (size_t i = 0; i < node->m_childs.size(); i++)
			{
				maxLeafNodes.push_back(&(node->m_childs[i]));
			}
		}
		else
		{
			palettesNode.push_back(node);
		}
		make_heap(maxLeafNodes.begin(), maxLeafNodes.end(), ColorNode_lessByLeafNum);
	}

	for (size_t i = 0; i < palettesNode.size(); i++)
	{
		palettes[i] = palettesNode[i]->m_color;
	}

	clock_t afterGetPalette = clock();

#define GetFloatCost(start, end) ((end) - (start)) / 1000.0f
	printf("total cost : %3f, sort cost: %3f, insert cost: %3f, palette cost: %3f\n",
		GetFloatCost(start, afterGetPalette),
		GetFloatCost(start, sortEnd),
		GetFloatCost(sortEnd, insertEnd),
		GetFloatCost(insertEnd, afterGetPalette));

	return palettesNode.size();
}
