#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>
#include "sqlite3.h"

#if !defined(CQ_ASSERT)
#define CQ_ASSERT(o) {if (!(o)) {__debugbreak(); printf("%s, %d\n", __FILE__, __LINE__);}}
#endif

#if !defined(CQ_LOG)
FILE* fLog;
#define CQ_LOG(format, ...) {\
	fprintf(fLog, format, __VA_ARGS__); \
	printf(format, __VA_ARGS__);\
}
#endif

using namespace std;

inline size_t getFileSize(FILE* fp)
{
	long fileSize, tempSeek;
	if (fp == NULL){
		return 0;
	}

	tempSeek = ftell(fp);
	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp);
	fseek(fp, tempSeek, SEEK_SET);
	return fileSize;
}

static bool readFile2Buffer(const wchar_t* fname, std::vector<char>& buffer)
{
	size_t fileLen;
	FILE* fp = _wfopen(fname, L"rb");
	if (fp == NULL){
		return false;
	}

	fileLen = getFileSize(fp);
	buffer.resize(fileLen);
	if (fileLen != 0)
	{
		fread(&buffer[0], 1, fileLen, fp);
	}
	fclose(fp);
	return true;
}

class PngWriter
{
public:
	bool open(const wchar_t* dbName)
	{
		char* errorMessage;
		sqlite3_open("sqlite3_test.db", &m_dbHandle);
		sqlite3_exec(m_dbHandle, "Create Table test (ID INT KEY, DATA BLOB)", NULL, NULL, &errorMessage);
		//sqlite3_exec(m_dbHandle, "PRAGMA automatic_index = true;PRAGMA page_size = 65536", NULL, NULL, NULL);
		sqlite3_prepare_v2(m_dbHandle, "insert into test(ID, DATA) values(:id,:data)", 100, &m_writerStmt, NULL);
		sqlite3_prepare_v2(m_dbHandle, "select gridData from test where ID=:id", 100, &m_selectStmt, NULL);
		return true;
	}

	void close()
	{
		sqlite3_finalize(m_selectStmt);
		sqlite3_finalize(m_writerStmt);
		sqlite3_close(m_dbHandle);
	}

	void beginTransaction()
	{
		char* errorMessage = NULL;
		sqlite3_exec(m_dbHandle, "Begin Transaction", NULL, NULL, &errorMessage);
		if (errorMessage != NULL){
			CQ_LOG("begin transaction error : %s\n", errorMessage);
			CQ_ASSERT(0);
		}
	}

	void endTransaction()
	{
		char* errorMessage = NULL;
		sqlite3_exec(m_dbHandle, "Commit Transaction", NULL, NULL, &errorMessage);
		if (errorMessage != NULL){
			CQ_LOG("end transaction error : %s\n", errorMessage);
			CQ_ASSERT(0);
		}
	}

	bool select(int key)
	{
		sqlite3_bind_int(m_writerStmt, sqlite3_bind_parameter_index(m_writerStmt, ":id"), key);
		sqlite3_step(m_writerStmt);
		//sqlite3_get_auxdata()
		//sqlite3_result_blob()
		sqlite3_reset(m_writerStmt);
		return true;
	}

	bool select(const char* keys)
	{
		char* selectCmd = (char*)malloc(100 + strlen(keys));
		sprintf(selectCmd, "select * from test where ID in(%s)", keys);
		clock_t start = clock();
		sqlite3_exec(m_dbHandle, selectCmd, NULL, NULL, NULL);
		printf("cost : %8f second\n", (clock() - start) / 1000.0f);
		return true;
	}

	bool write(int key, void* data, size_t dataLen)
	{
		int index1 = sqlite3_bind_parameter_index(m_writerStmt, ":id");  
		int index2 = sqlite3_bind_parameter_index(m_writerStmt, ":data"); 
		sqlite3_bind_int(m_writerStmt, index1, key);
		sqlite3_bind_blob(m_writerStmt, index2, data, dataLen, NULL);
		sqlite3_step(m_writerStmt);
		sqlite3_reset(m_writerStmt);
		return true;
	}

private:
	sqlite3* m_dbHandle;
	sqlite3_stmt* m_writerStmt;
	sqlite3_stmt* m_selectStmt;
};


int main()
{
	vector<char> buffer;
	fLog = _wfopen(L"log.txt", L"w+");
	readFile2Buffer(L"0200169L.jpg", buffer);

	PngWriter writer;
	writer.open(L"sqlite3_test.db");
	CQ_LOG("Number, Cost Period, Cost Total\n");

	clock_t start = clock();
	clock_t lastTimestamp = start;

	int commitSize = 1000;
	for (int i = 0; i < 200000; i+=commitSize)
	//for (int i = 0; i < 12000000; i += commitSize)
	{
		writer.beginTransaction();
		for (int j = 0; j < commitSize; j++)
		{
			writer.write(i + j, &buffer[0], buffer.size());
		}
		writer.endTransaction();

		// show cost time 
		clock_t currentTimestamp = clock();
		CQ_LOG("%8d, %8f, %8f\n", i+commitSize, (currentTimestamp - lastTimestamp) / 1000.0f, (currentTimestamp - start) / 1000.0f);
		lastTimestamp = currentTimestamp;
	}

	start = clock();
	for (int i = 0; i < 100000; i+=3)
	{
		writer.select(i);
	}
	printf("select cost : %8f second\n", (clock() - start) / 1000.0f);

	string cmd;
	char buff[20];
	for (int i = 0; i < 100000; i+=3)
	{
		cmd += buff;
		//cmd = cmd + string(i == 0? "":",") + string(itoa(i, buff, 10));
	}
	writer.select(cmd.c_str());

	writer.close();

	CQ_LOG("Test complete cost: %8f second\n", (clock() - start) / 1000.0f);
	fclose(fLog);
	return 0;
}