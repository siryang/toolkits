#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>
#include "sqlite3.h"
#include "common_util.h"

#define PI 3.14

using namespace std;
using namespace common;

template<typename Type>
inline void deletePointer(Type* p)
{
	delete p;
}

class PngWriter
{
public:
	bool open(const wchar_t* dbName)
	{
		char* errorMessage;
		sqlite3_open("sqlite3_test.db", &m_dbHandle);
		sqlite3_exec(m_dbHandle, "Create Table test (ID INT KEY, DATA BLOB)", NULL, NULL, &errorMessage);
		return true;
	}

	void beginTransaction()
	{
		char* errorMessage = NULL;
		sqlite3_exec(m_dbHandle, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);
		if (errorMessage != NULL){
			cout << errorMessage << endl;
		}
	}

	void endTransaction()
	{
		char* errorMessage = NULL;
		sqlite3_exec(m_dbHandle, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
		if (errorMessage != NULL){
			cout << errorMessage << endl;
		}
	}

	bool write(int key, void* data, size_t dataLen)
	{
		sqlite3_prepare_v2(m_dbHandle, "insert into test(ID, DATA) values(:id,:data)", 100, &m_writerStmt, NULL);
		int index1 =  sqlite3_bind_parameter_index(m_writerStmt, ":id");  
		int index2 =  sqlite3_bind_parameter_index(m_writerStmt, ":data"); 
		sqlite3_bind_int(m_writerStmt, index1, key);
		sqlite3_bind_blob(m_writerStmt, index2, data, dataLen, NULL);
		sqlite3_step(m_writerStmt);
		return true;
	}

	void close()
	{
		sqlite3_close(m_dbHandle);
	}
private:
	sqlite3* m_dbHandle;
	sqlite3_stmt* m_writerStmt;
};

int main()
{
	clock_t start = clock();
	clock_t lastTimestamp = start;
	vector<char> buffer;
	readFile2Buffer(L"test.png", buffer);

	PngWriter writer;
	writer.open(L"sqlite3_test.db");
	writer.beginTransaction();
	cout << "Number, Cost Period, Cost Total" << endl;
	//for (int i = 0; i < 200000; i++)
	for (int i = 0; i < 21000000; i++)
	{
		bool commitTransaction = i % 5000 == (5000 - 1);
		writer.write(i, &buffer[0], buffer.size());
		if (commitTransaction){
			clock_t currentTimestamp = clock();
			writer.endTransaction();
			
			// show time cost
			char buffer[256];
			sprintf(buffer, "%8d, %8f, %8f\n", i+1, (currentTimestamp - lastTimestamp) / 1000.0f, (currentTimestamp - start) / 1000.0f);
			cout << buffer;

			lastTimestamp = currentTimestamp;
			writer.beginTransaction();
		}
	}
	writer.endTransaction();
	writer.close();

	cout << "Test complete cost:"<< (clock() - start) / 1000.0f << " second" << endl;
	return 0;
}