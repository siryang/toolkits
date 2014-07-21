#pragma once
#include <vector>

namespace common
{
// write png rgba
bool writePng(const char* bitmap, size_t pixelSize, int width, int height, const char *path);

bool readPng( const wchar_t* fname, int* width, int* height, std::vector<char>& pngBuffer);
}
