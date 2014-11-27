#include <iostream>
#include <signal.h>

using namespace std;

void catcher(int sig)
{
	cout << "get signal " << sig << endl;
}
int main()
{
	struct sigaction sigact;
	sigset_t sigset;

	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = 0;
	sigact.sa_handler = catcher;
	sigaction(SIGUSR1, &sigact, NULL);

	cout << "raise signal" << endl;
	raise(SIGUSR1);

	sigemptyset(&sigset);
	sigaddset(&sigset, SIGUSR1);
	sigprocmask(SIG_SETMASK, &sigset, NULL);
	
	cout << "raise signal" << endl;
	raise(SIGUSR1);
	cout << "raise over" << endl;
	return 1;
}

