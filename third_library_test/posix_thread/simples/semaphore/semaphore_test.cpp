#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>

using namespace std;

/*
init:read: 0, write: 1

p(write)
write...
v(read)

p(read)
read...
v(write)
*/

pthread_t writeThread, readThread; 
sem_t semCouldRead;
sem_t semCouldWrite;

int tempValue = 0;
char buffer[1024] = "init";

void* thread_write(void* arg)
{
	while(tempValue < 10)
	{
		sem_wait(&semCouldWrite);
		sprintf (buffer, "Current Value: %d", tempValue++ );
		cout << "Writing..." << endl;
		sem_post(&semCouldRead);	
	}	
	cout << "Write thread exit" << endl;
	pthread_exit(0);
}

void* thread_read(void* arg)
{
	//pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	while(1)
	{
		sem_wait(&semCouldRead);
		
		//pthread_testcancel();
		cout << "Read... " << endl
		 << buffer << endl;

		sem_post(&semCouldWrite);
	}
}

int main()
{
	sem_init(&semCouldRead, 0, 0);	
	sem_init(&semCouldWrite, 0, 1);	

    pthread_create(&readThread, NULL, thread_read, NULL);
    pthread_create(&writeThread, NULL, thread_write, NULL);

    pthread_join(writeThread, NULL);	
	
	pthread_cancel(readThread);
	sem_post(&semCouldRead);

    pthread_join(readThread, NULL);

	sem_destroy(&semCouldRead);
	sem_destroy(&semCouldWrite);
    return 0;
}


