#include "thread_pool.h"
#include <iostream>

using namespace std;
#ifdef _WIN32
#include <Windows.h>
#endif

#ifndef CQ_ASSERT
#define CQ_ASSERT(o) {if (!(o)) {*(int*)0 = 0;}}
#endif

class CalcTask
	:public ThreadTask
{
public:
	int a, b, c;
	int* dest;
};

typedef void (*Callback)(int*);

class CalcWorker
	:public ThreadWorker
{
public:
	CalcWorker()
	{
		setCallBack();
	}

	void setCallBack()
	{
		m_callBack = addInt;
	}

	static void addInt(int* v)
	{
		*v += 2;
	}

	virtual void beforeWork()
	{
		// init in thread.
		printf ("init in thread, id: %d\n", pthread_self());
	}

	virtual void doWork()
	{
		CalcTask* task = reinterpret_cast<CalcTask*>(m_task);
		*task->dest = task->a * task->b * task->c;
		if (m_callBack != NULL)
		{
			m_callBack(task->dest);
		}
	}

	virtual void afterWork()
	{
		// clean thread.
		printf ("cleanup in thread, id %d\n", pthread_self());
	}

private:
	Callback m_callBack;
};

struct TaskOption
{
	ThreadPool* pool;
	int* dest;
	size_t taskNum;
};

void* addTask(void* arg)
{
	TaskOption* options = reinterpret_cast<TaskOption*>(arg);

	ThreadPool* pool = options->pool;
	int* dest = options->dest;
	size_t taskNum = options->taskNum;

	size_t i;
	for (i = 0; i < taskNum / 2; i++)
	{
		CalcTask* tasks = new CalcTask;
		tasks->a = i;
		tasks->b = i;
		tasks->c = i;
		tasks->dest = dest + i;
		pool->addTask(tasks);
	}

	for (; i < taskNum; i++)
	{
		CalcTask* tasks = new CalcTask;
		tasks->a = i;
		tasks->b = i;
		tasks->c = i;
		tasks->dest = dest + i;
		pool->addTask(tasks);
	}
	return NULL;
}

int main()
{
	const size_t taskNum = 10000;
	size_t testThreadNum = 20;
	size_t poolThreadNum = 10;
	ThreadPool tdPool;

	vector<ThreadWorker*> workers;
	for (size_t j = 0; j < poolThreadNum; j++)
	{
		ThreadWorker* worker = new CalcWorker;
		workers.push_back(worker);
		tdPool.addWorker(worker);
	}

	pthread_t* testThreadId = new pthread_t[testThreadNum];
	TaskOption* options = new TaskOption[testThreadNum];
	for (size_t j = 0; j < testThreadNum; j++)
	{
		options[j].taskNum = taskNum;
		options[j].pool = &tdPool;
		options[j].dest = new int[taskNum];
		pthread_create(testThreadId + j, NULL, addTask, &options[j]);
	}
	
	if (!tdPool.start())
	{
		cout << "run thread pool fail!" << endl;
		goto func_return;
	}

	for (size_t j = 0; j < testThreadNum; j++)
	{
		pthread_join(testThreadId[j], NULL);
	}

	delete[] testThreadId;


	tdPool.waitAndExit();
	tdPool.stopAndExit();

	for (size_t j = 0; j < testThreadNum; j++)
	{
		int* temp = options[j].dest;
		for (size_t i = 0; i < taskNum; i++)
		{
			//cout << dest[i] << ",";
			if (temp[i] != (int)(i * i * i + 2))
			{
				cout << "error in thread pool" << endl;
				CQ_ASSERT(0);
			}
		}
	}
	
	cout << "test complete!" << endl;

#ifdef _WIN32
	__debugbreak();
#endif

func_return:
	for (size_t i = 0; i < poolThreadNum; i++)
	{
		delete workers[i];
	}
	
	for (size_t j = 0; j < testThreadNum; j++)
	{
		delete[] options[j].dest;
	}
	delete[] options;
	return 0;
}
