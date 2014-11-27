#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS  3
#define TCOUNT 10
#define COUNT_LIMIT 12

#ifdef WIN32
#define sleep(o) Sleep((o) * 1000)
#endif

int     count = 0;
int     thread_ids[3] = {0,1,2};
pthread_mutex_t mutexLock;
pthread_cond_t condLock;

void* sendCond(void *t) 
{
  int i;
  long tId = (long)t;

  for (i=0; i<TCOUNT; i++)
  {
    pthread_mutex_lock(&mutexLock);
    count++;

	// when count == COUNT_LIMIT send cond_signal.
    if (count == COUNT_LIMIT) 
	{
      pthread_cond_signal(&condLock);
      printf("sendCond(): thread %ld, count = %d  Threshold reached.\n", 
             tId, count);
    }

    printf("sendCond(): thread %ld, count = %d, unlocking mutex\n", 
	   tId, count);
    pthread_mutex_unlock(&mutexLock);

    /* Do some "work" so threads can alternate on mutex lock */
    sleep(1);
  }
	
  pthread_exit(NULL);
}

void* waitCond(void *t) 
{
  long tId = (long)t;

  printf("Starting waitCond(): thread %ld\n", tId);

  pthread_mutex_lock(&mutexLock);
  while (count < COUNT_LIMIT) 
  {
	// wait count_signal
    pthread_cond_wait(&condLock, &mutexLock);
    printf("waitCond(): thread %ld Condition signal received.\n", tId);
    count += 125;
    printf("waitCond(): thread %ld count now = %d.\n", tId, count);
  }
  pthread_mutex_unlock(&mutexLock);
  pthread_exit(NULL);
}

int main ()
{
  int i;
  long t1=1, t2=2, t3=3;
  pthread_t threads[3];

  /* Initialize mutex and condition variable objects */
  pthread_mutex_init(&mutexLock, NULL);
  pthread_cond_init (&condLock, NULL);

  /* For portability, explicitly create threads in a joinable state */
  pthread_create(&threads[0], NULL, waitCond, (void *)t1);
  pthread_create(&threads[1], NULL, sendCond, (void *)t2);
  pthread_create(&threads[2], NULL, sendCond, (void *)t3);

  /* Wait for all threads to complete */
  for (i = 0; i < 3; i++)
  {
    pthread_join(threads[i], NULL);
  }

  printf ("Main(): Waited on %d  threads. Done.\n", NUM_THREADS);

  /* Clean up and exit */
  pthread_mutex_destroy(&mutexLock);
  pthread_cond_destroy(&condLock);
  return 0;
}

