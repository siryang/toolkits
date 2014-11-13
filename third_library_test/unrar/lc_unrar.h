#pragma once

#include <vector>
#include <string>

using namespace std;

class RarReaderImp;

class RarReader
{
public:
	RarReader();
	~RarReader();

	bool open(wchar_t* filename);
	void close();
	void getFileList(vector<wstring>& fileList);
	void getFileList(vector<string>& fileList);

private:
	RarReaderImp* m_imp;
	vector<wstring> m_filelist;
};