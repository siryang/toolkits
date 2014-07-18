#include "grid_data_parser.h"
#include "common_util.h"

bool GridDataParser::parse( GridId gridId, ImageInfo& png, void* data, size_t dataLen )
{
	gridId;
	data;
	dataLen;
	common::readPng(L"heart.png", &png.width, &png.height, png.data);
	return true;
}
