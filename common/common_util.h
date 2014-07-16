#pragma once
#include <vector>

namespace common
{
#define CM_ASSERT(o) {if (!(o)) {__debugbreak(); printf("%s, %d\n", __FILE__, __LINE__);}}
inline size_t getFileSize(FILE* fp);
bool readFile2Buffer(const wchar_t* fname, std::vector<char>& buffer);

// write png rgba
bool writePng(const unsigned char* bitmap, size_t pixelSize, size_t width, size_t height, const char *path);

bool readPng( const char* fname, size_t* width, size_t* height, std::vector<char>& pngBuffer);
}
