#pragma once
#include <vector>

namespace common
{
inline size_t getFileSize(FILE* fp);
bool readFile2Buffer(const wchar_t* fname, std::vector<char>& buffer);
}
