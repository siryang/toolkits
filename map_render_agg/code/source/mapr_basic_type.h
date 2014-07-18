#pragma once
#include <vector>

typedef unsigned int GridId;

struct ImageInfo
{
	int width;
	int height;
	std::vector<char> data;
};
