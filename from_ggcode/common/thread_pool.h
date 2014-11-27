#ifndef _THREAD_POOL_
#define _THREAD_POOL_

#include <queue>
#include <pthread.h>

using namespace std;

typedef void ThreadTask;
class ThreadPool;
class ThreadWorker
{
	friend class ThreadPool;
public:
	enum WorkMode{
		WorkMode_standard,
		WorkMode_loop
	};

	ThreadWorker(ThreadPool* pool)
		:m_pool(pool)
	{}
	
	~ThreadWorker()
	{
		pthread_cancel(m_threadId);
		waitForComplete();
	}
	
	virtual void doTask() = 0;

private:
	inline void start();
	inline static void exit();
	inline void waitForComplete();
	static void* workLoop(void* worker);

protected:
	pthread_t m_threadId;
	ThreadTask* m_task;
	ThreadPool* m_pool;
	WorkMode m_workMode;
};

typedef std::queue<ThreadTask*> ThreadTaskQueue;
class ThreadPool
{
public:
	ThreadPool();
	~ThreadPool();

	bool start();
	void exit();

	bool setWorkers(ThreadWorker* worker, size_t num);
	ThreadWorker* dismissWorkers();

	/// delete task by worker. so should alloc task at heap memory.
	void addTask(ThreadTask* task);
	ThreadTask* getNextTask();

	void waitForAllTaskComplete();

	inline bool isRunning()
	{
		return m_running;
	}

private:
	ThreadTaskQueue m_taskList;
	ThreadWorker* m_workers;
	size_t m_workersNum;

	pthread_mutex_t m_taskListLock;
	pthread_cond_t m_taskReady;
	bool m_running;
	bool m_nomoreTask;
};

#endif

