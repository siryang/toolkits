#include "lc_unrar.h"
#include <Windows.h>
#define UNRAR
#define SILENT
#define RARDLL
#include "dll.hpp"
#pragma comment(lib, "unrar_x64.lib")

class RarReaderImp
{
public:
	RarReaderImp();

	bool open(wchar_t* filename);
	void close();
	void getFileList(vector<wstring>& filelist);
	char* loadFile(wchar_t* filename, size_t* len);
private:

	HANDLE rarFile;
	RARHeaderDataEx fileInfo;
	RAROpenArchiveDataEx archiveInfo;
};

RarReaderImp::RarReaderImp()
{
	memset(&archiveInfo, 0, sizeof(archiveInfo));
	archiveInfo.CmtBuf = NULL;
	archiveInfo.OpenMode = RAR_OM_LIST;
	//archiveInfo.OpenMode = RAR_OM_EXTRACT;
}

bool RarReaderImp::open(wchar_t* filename)
{
	archiveInfo.ArcNameW = filename;
	rarFile = RAROpenArchiveEx(&archiveInfo);
	if (archiveInfo.OpenResult != 0) {
		RARCloseArchive(rarFile);
		return false;
	}
	return true;
}

void RarReaderImp::close()
{
	RARCloseArchive(rarFile);
}

void RarReaderImp::getFileList(vector<wstring>& filelist)
{
	int RHCode;
	int numFiles = 0;
	fileInfo.CmtBuf = NULL;
	while(true) 
	{
		RHCode = RARReadHeaderEx(rarFile, &fileInfo);
		if (RHCode != 0)
			break;

		filelist.push_back(fileInfo.FileNameW);

		if (RARProcessFile(rarFile, RAR_SKIP, NULL, NULL) != 0)
			break;
	}
		
}

char* RarReaderImp::loadFile( wchar_t* filename, size_t* len )
{
	//RARProcessFile(rarFile, RAR_EXTRACT, currDir, tmpFile);
	return NULL;
}

RarReader::RarReader()
{
	m_imp = new RarReaderImp;
}

RarReader::~RarReader()
{
	delete m_imp;
}

bool RarReader::open( wchar_t* filename )
{
	m_filelist.clear();
	return m_imp->open(filename);
}

void RarReader::close()
{
	m_imp->close();
	m_filelist.clear();
}

void RarReader::getFileList( vector<wstring>& fileList )
{
	if (m_filelist.empty()){
		m_imp->getFileList(m_filelist);
	}
	fileList = m_filelist;
}
