#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

using namespace std;

pthread_t id1, id2; 

int pthread_getThreadId()
{
#ifdef WIN32
	return (int)pthread_self().p;
#else
	return pthread_self();
#endif
}

void* thread_task(void* arg)
{
	const char tdMagicWord[] = "Hello world by thread--";
	char* result = new char[sizeof(tdMagicWord) + 20];
	
	cout << "PID:" << getpid() 
		<< "\tTID:" << pthread_getThreadId() << endl;
			
	strcpy(result, tdMagicWord);
	sprintf(result + sizeof(tdMagicWord) - 1, "%d", pthread_self());

	pthread_exit(result);
}

int main()
{
	pthread_attr_t tdAttr;
	char* tdResult[2];
	size_t stackSize = 0;
	
	pthread_attr_init(&tdAttr);
	pthread_attr_getstacksize(&tdAttr, &stackSize);
	cout << "Thread default stack size:" << stackSize << endl;
	//			Thread ID, attr, function, arg
    pthread_create(&id1, NULL, thread_task, NULL);
    pthread_create(&id2, NULL, thread_task, NULL);

	/*
	detach: 分离线程
		除非线程分离，否则在线程退出时，不会释放它的资源。
		分离线程，在执行完毕后自己释放资源，不需要其他线程等待其结束。
	Join: 回收线程
		获取非分离线程的返回值，并释放资源。
	*/
    pthread_join(id1, (void**)&tdResult[0]);
    pthread_join(id2, (void**)&tdResult[1]);
	
	cout << "Return value of Td1:" << tdResult[0] << endl
		<< "Return value of Td2: " << tdResult[1] << endl;

	delete[] tdResult[0];
	delete[] tdResult[1];
    return 0;
}


