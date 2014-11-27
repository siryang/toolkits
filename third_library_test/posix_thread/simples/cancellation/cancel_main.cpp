#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <cstdio>
/*
  Posix里面，会引起阻塞的系统调用都是Cancelation-point。		
  对于C库函数来说，几乎可以使线程挂起的函数都会响应CANCEL信号，终止线程，包括sleep、delay等延时函数。
	pthread_cancle(): 发送取消信号			
	pthread_testcancle(): 手动设置取消点			
	pthread_setcancelstate: 设置是否允许在响应取消信号。PTHREAD_CANCEL_ENABLE、PTHREAD_CANCEL_DISABLE  		
	pthread_setcanceltype:  设置线程退出方式。PTHREAD_CANCEL_DEFERRED（只能在取消点上响应退出信号，默认）、PTHREAD_CANCEL_ASYNCHRONOUS（线程在任何时候都可以响应退出信号）
*/

pthread_t id1, id2; 

void* thread_task(void* arg)
{
	int threadId = pthread_equal(pthread_self(), id1)? 1: 2;
	int waitTime = 0;
	if (threadId == 1)
	{
		// enable cancle for thread:id1
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
		pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	}
	else
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	
	while(1)
	{
		waitTime++;
		if (waitTime == 3)
		{	
			pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
		}
		else if (waitTime == 100 && threadId != 1)
		{
			// printf will create a cancellation.
			printf ("Thread: %d, wait 100 loop\n", threadId);
		}
		else if (waitTime == 1000)
		{
			printf("Tread:%d, will not end.\n", threadId);
			pthread_exit(0);
		}
	}
}

int main()
{
    pthread_create(&id1, NULL, thread_task, NULL);
    pthread_create(&id2, NULL, thread_task, NULL);

	// must send cancellation to thread;	
	pthread_cancel(id1);
	pthread_cancel(id2);

    pthread_join(id1, NULL);
    pthread_join(id2, NULL);
    return 0;
}


