#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <cstdio>

/*
	TLS: Thread Local Storage
	thread 1 set tlsTest++.
	thread 2 read tlsTest after 1 second.
*/
__thread int tlsTest = 0;
pthread_t id1, id2; 

void* thread_task(void* arg)
{
    pid_t pid = getpid();
	int tId = pthread_equal(pthread_self(), id1)? 1: 2;
    if (tId == 1)
	{
		tlsTest++;
	}
	else
	{
		sleep(1);
	}

	printf ("thread id: %d, tls: %d\n", tId, tlsTest);
	return NULL;
}

int main()
{
    pthread_create(&id1, NULL, thread_task, NULL);
    pthread_create(&id2, NULL, thread_task, NULL);

    pthread_join(id1, NULL);
    pthread_join(id2, NULL);
    return 0;
}


