#include <signal.h>
#include <iostream>

using namespace std;
#define SIGUSR2 12

void signalCallBack(int handle)
{
	switch(handle)
	{
	case SIGSEGV:
		cout << "Segment falut: " << handle << endl;
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
		cout << "Hello All" << endl;
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

#if !defined (WIN32)
	userDefinedSignalTest();
#endif

	signal(SIGSEGV, signalCallBack);
	*(int*)0 = 3;

	cout << "Main Exit" << endl;
	return 0;
}
