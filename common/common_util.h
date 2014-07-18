#pragma once
#include <vector>

namespace common
{
#define CM_ASSERT(o) {if (!(o)) {__debugbreak(); printf("%s, %d\n", __FILE__, __LINE__);}}
inline size_t getFileSize(FILE* fp);
bool readFile2Buffer(const wchar_t* fname, std::vector<char>& buffer);

// write png rgba
bool writePng(const char* bitmap, size_t pixelSize, int width, int height, const char *path);

bool readPng( const wchar_t* fname, int* width, int* height, std::vector<char>& pngBuffer);
}
