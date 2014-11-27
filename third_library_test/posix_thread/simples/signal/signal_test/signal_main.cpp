#include <signal.h>

#include <iostream>

using namespace std;

void signalCallBack(int handle)
{
	switch(handle)
	{
	case SIGSEGV:
		cout << "Segment falut: " << handle << endl;
		exit(0);
		break;
	case SIGINT:
		cout << "Signal INT:" << handle << endl;
		break;
	default:
		break;
	}
}

void userSignalCallBack(int handle)
{
	switch(handle)
	{
	case SIGUSR2:
		cout << "Signal USR2:" << handle << endl;
		break;
	default:
		break;
	}
}

void userDefinedSignalTest()
{
	signal(SIGUSR2, userSignalCallBack);
	raise(SIGUSR2);
}

int main()
{
	signal(SIGINT, signalCallBack);
	raise(SIGINT);

	signal(SIGINT, SIG_IGN);
	raise(SIGINT);

	signal(SIGINT, SIG_DFL);

	userDefinedSignalTest();

	signal(SIGSEGV, signalCallBack);
	*(int*)0 = 3;

	cout << "Main Exit" << endl;
	return 0;
}


