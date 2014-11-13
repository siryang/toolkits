#include "stdafx_evfc.h"
#include "gtest/gtest.h"
#include "lc_unrar.h"

TEST(unrar, filelist)
{
	RarReader reader;
	reader.open(L"compiler_evfc.rar");

	vector<wstring> filelist;
	reader.getFileList(filelist);
	
	for each(const wstring& filename in filelist)
		wcout << filename << endl;

	reader.close();
}