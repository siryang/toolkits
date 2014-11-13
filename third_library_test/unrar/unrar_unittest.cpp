#include "gtest/gtest.h"
#include <Windows.h>
#include <iostream>
#include "../dll.hpp"

using namespace std;


int CALLBACK CallbackProc(UINT msg,LPARAM myBufferPtr,LPARAM rarBuffer,LPARAM bytesProcessed) {
	switch(msg) {
	case UCM_CHANGEVOLUME:
		return -1;
		break;
	case UCM_PROCESSDATA:
		memcpy(*(char**)myBufferPtr, (char*)rarBuffer, bytesProcessed);
		*(char**)myBufferPtr += bytesProcessed;
		return 1;
		break;
	case UCM_NEEDPASSWORD:
		return -1;
		break;
	}
	return -1;
}

TEST(UnrarTest, open)
{
	HANDLE rarFile;  // I renamed this macro in dll.hpp for my own purposes
	HANDLE rarFile2;
	RARHeaderDataEx fileInfo;
	RAROpenArchiveDataEx archiveInfo;
	memset(&archiveInfo, 0, sizeof(archiveInfo));
	archiveInfo.CmtBuf = NULL;
	//archiveInfo.OpenMode = RAR_OM_LIST;
	archiveInfo.OpenMode = RAR_OM_EXTRACT;
	archiveInfo.ArcName = "gtest.rar";

	// Open file
	rarFile = RAROpenArchiveEx(&archiveInfo);
	rarFile2 = RAROpenArchiveEx(&archiveInfo);
	if (archiveInfo.OpenResult != 0) {
		RARCloseArchive(rarFile);
		cout  << "unrar couldn't open" << endl;
		exit(1);
	}
	fileInfo.CmtBuf = NULL;

	//  cout  << archiveInfo.Flags << endl;

	// loop through archive
	int numFiles = 0;
	int fileSize;
	int RHCode;
	int PFCode;
	int crcVal;
	bool workaroundUsed = false;
	char currDir[2] = ".";
	char tmpFile[11] = "buffer.tmp";
	while(true) {
		RHCode = RARReadHeaderEx(rarFile, &fileInfo);
		if (RHCode != 0) break;
		RARReadHeaderEx(rarFile2, &fileInfo);

		numFiles++;
		//fs::path path(fileInfo.FileName);
		fileSize = fileInfo.UnpSize;
		crcVal = fileInfo.FileCRC;

		cout << dec << fileInfo.Method << " " << fileInfo.FileName << " (" << fileInfo.UnpSize << ")" << endl;
		cout << " " << hex << uppercase << crcVal << endl;

		char* fileBuffer = (char*)malloc(fileSize);
		char* bufferPtr = fileBuffer;

		// not sure what this does
		//RARSetProcessDataProc(rarFile, ProcessDataProc);

		// works for some files, but not for others
		RARSetCallback(rarFile, CallbackProc, (long) &bufferPtr);
		PFCode = RARProcessFile(rarFile, RAR_TEST, NULL, NULL);

		// properly extracts to a directory... but I need a stream
		// and I don't want to write to disk, read it, and delete from disk
		//    PFCode = RARProcessFile(rarFile, RAR_EXTRACT, currDir, fileInfo.FileName);

		// just skips
		//PFCode = RARProcessFile(rarFile, RAR_SKIP, NULL, NULL);

		if (PFCode != 0) {
			RARCloseArchive(rarFile);
			cout  << "error processing this file\n" << endl;
			exit(1);
		}

		//// crc check
		//boost::crc_32_type crc32result;
		//crc32result.process_bytes(&fileBuffer, fileSize);
		//cout << " " << hex << uppercase << crc32result.checksum() << endl;

		// old workaround - crc check always succeeds now!
		//if (crcVal == crc32result.checksum()) {
		//	RARProcessFile(rarFile2, RAR_SKIP, NULL, NULL);
		//}
		//else 
		{
			workaroundUsed = true;
			RARProcessFile(rarFile2, RAR_EXTRACT, currDir, tmpFile);
			//ifstream inFile(tmpFile);
			//inFile.read(fileBuffer, fileSize);
		}

		//ofstream outFile(path.filename().c_str());
		//outFile.write(fileBuffer, fileSize);
	}
	if (workaroundUsed) remove(tmpFile);
	if (RHCode != ERAR_END_ARCHIVE)
		cout  << "error traversing through archive: " << RHCode << endl;
	RARCloseArchive(rarFile);

	cout  << dec << "num files: " << numFiles << endl;

}