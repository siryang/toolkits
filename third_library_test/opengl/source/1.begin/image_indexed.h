#pragma once

#include "cq_types_basic.h"

BOOL savePng(const cqWCHAR* filename, void* pngBuffer, int width, int height, int pitch);
void testPng();

/**
// �����ɫ�塢PngBuffer�ڴ�
ImageIndexed_init();,

ImageIndexed_cleanup();

ImageIndexed_encodePng(void* pngBuffer, int width, int height, size_t colorNum)

*/
