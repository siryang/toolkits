#pragma once

extern "C"
{
#include "cq_types_basic.h"
};

int extractPalete_octree(PixelType* buffer, int width, int height, int pitchInPixel, PixelType* palettes, int maxPaletteNum);

