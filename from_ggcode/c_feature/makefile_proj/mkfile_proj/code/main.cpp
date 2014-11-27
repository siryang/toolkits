#include <iostream>
#include <cstdlib>

extern "C"
{
	#include <pthread.h>
};

using namespace std;

void* threadFunc(void* arg)
{
	arg = arg;
	cout << "This is a test of thread" << endl;
	return NULL;
}

int main()
{
	cout << "Hello Makefile" << endl;

	pthread_t testThread;
	pthread_create(&testThread, NULL, threadFunc, NULL);
	pthread_join(testThread, NULL);
	system("pause");
	return 1;
}