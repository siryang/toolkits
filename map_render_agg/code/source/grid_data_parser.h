#pragma once
extern "C"
{
	#include "cq_types_basic.h"
};

#include "png_productor.h"

class GridDataParser
{
public:
	bool parse( GridId gridId, ImageInfo& png, void* data, size_t dataLen );
};