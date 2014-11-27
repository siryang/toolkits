#include <iostream>
#include <signal.h>

using namespace std;
pthread_t tId;

void sigCallBack(int sigNum)
{
	cout << "Catch Signal:" << sigNum << endl;
	cout << "Thread Id " << pthread_self() << endl;
	exit(0);
}

void* threadFunction(void* arg)
{
	arg;
	signal(SIGINT, sigCallBack);
	while(true)
	{
		cout << "thread " << tId << " sleeping" << endl;
		sleep(10000);
		// 可以用pause()将线程挂起，直到收到一个信号为止。
		cout << "Sleep over" << endl;
	}
}

int main()
{
	pthread_create(&tId, NULL, threadFunction, NULL);
	sleep(2);
	cout << "raise signal to thread" << endl;
	pthread_kill(tId, SIGINT);
	pthread_join(tId, NULL);
	return 0;
}


