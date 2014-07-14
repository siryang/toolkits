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
		sqlite3_prepare_v2(m_dbHandle, "insert into test(ID, DATA) values(:id,:data)", 100, &m_writerStmt, NULL);
		return true;
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

	bool write(int key, void* data, size_t dataLen)
	{
		int index1 =  sqlite3_bind_parameter_index(m_writerStmt, ":id");  
		int index2 =  sqlite3_bind_parameter_index(m_writerStmt, ":data"); 
		sqlite3_bind_int(m_writerStmt, index1, key);
		sqlite3_bind_blob(m_writerStmt, index2, data, dataLen, NULL);
		sqlite3_step(m_writerStmt);
		sqlite3_reset(m_writerStmt);
		return true;
	}

	void close()
	{
		sqlite3_finalize(m_writerStmt);
		sqlite3_close(m_dbHandle);
	}
private:
	sqlite3* m_dbHandle;
	sqlite3_stmt* m_writerStmt;
};

int main()
{
	fLog = _wfopen(L"log.txt", L"w+");
	clock_t start = clock();
	clock_t lastTimestamp = start;
	vector<char> buffer;
	readFile2Buffer(L"test.png", buffer);

	PngWriter writer;
	writer.open(L"sqlite3_test.db");
	writer.beginTransaction();
	CQ_LOG("Number, Cost Period, Cost Total\n");
	//for (int i = 0; i < 200000; i++)
	for (int i = 0; i < 12000000; i++)
	{
		bool commitTransaction = i % 5000 == (5000 - 1);
		writer.write(i, &buffer[0], buffer.size());
		if (commitTransaction){
			clock_t currentTimestamp = clock();
			writer.endTransaction();
			
			// show cost time 
			CQ_LOG("%8d, %8f, %8f\n", i+1, (currentTimestamp - lastTimestamp) / 1000.0f, (currentTimestamp - start) / 1000.0f);

			lastTimestamp = currentTimestamp;
			writer.beginTransaction();
		}
	}
	writer.endTransaction();
	writer.close();

	CQ_LOG("Test complete cost: %8f second\n", (clock() - start) / 1000.0f);
	fclose(fLog);
	return 0;
}