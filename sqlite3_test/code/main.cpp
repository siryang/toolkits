#include <vector>
#include <iostream>
#include "sqlite3.h"
#include "common_util.h"

#define PI 3.14

using namespace std;


bool sqlite3_write(sqlite3* handle, int key, void* data, size_t dataLen)
{
	sqlite3_prepare(handle, "insert into sqlite3_test ...")
	return true;
}

int main()
{
	sqlite3* dbHandle;
	sqlite3_open("sqlite3_test.db", &dbHandle);
	vector<char> buffer;
	readFile2Buffer("test.png", buffer);
	// begin transaction

	//for (int i = 0; i < 20000000; i++)
	for (int i = 0; i < 200; i++)
	{
		bool commitTransaction = i % 1000 == 0;
		sqlite3_write(dbHandle, i, &buffer[0], buffer.size());
		if (commitTransaction){
			// end transaction
			// begin transaction
		}
	}
	// end transaction

	sqlite3_close(dbHandle);
	return 0;
}