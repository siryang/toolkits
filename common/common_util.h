#pragma once
#include <vector>

namespace common
{

#define CM_ASSERT(o) {if (!(o)) {__debugbreak(); printf("%s, %d\n", __FILE__, __LINE__);}}
	inline size_t getFileSize(FILE* fp);
	bool readFile2Buffer(const wchar_t* fname, std::vector<char>& buffer);


}
