#pragma once
#include <stdio.h>

inline size_t getFileSize(FILE* fp)
{
	long fileSize, tempSeek;
	if (fp == nullptr){
		return 0;
	}

	tempSeek = ftell(fp);
	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp);
	fseek(fp, tempSeek, SEEK_SET);
	return fileSize;
}

bool readFile2Buffer(const wchar_t* fname, vector<char>& buffer)
{
	size_t fileLen;
	FILE* fp = _wfopen(fname, "rb");
	if (fp == nullptr){
		return false;
	}

	fileLen = getFileSize(fp);
	buffer.resize(fileLen);
	if (fileLen != 0)
	{
		fread(&buffer[0], 1, fileLen, fp);
	}
	fclose(fp);
}