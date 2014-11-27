#ifndef _THREAD_POOL_
#define _THREAD_POOL_

#ifdef _WIN32

#include <windows.h>

#if !defined(sleep)
#define sleep(a) Sleep((a) * 1000)
#endif

#endif

#include <queue>
#include <pthread.h>
using namespace std;

class ThreadTask{
public:
	virtual ~ThreadTask(){}
};
class ThreadPool;
class ThreadWorker
{
	friend class ThreadPool;
public:
	enum WorkMode{
		WorkMode_standard,
		WorkMode_loop
	};

	ThreadWorker(): m_workMode(WorkMode_standard), m_task(NULL), m_pool(NULL)
	{}
	
	virtual ~ThreadWorker();

	// thread init
	virtual void beforeWork(){};
	// thread cleanup
	virtual void afterWork(){};

	virtual void doWork() = 0;

private:
	inline bool start();
	static void exit();
	inline void waitForComplete();
	static void* workLoop(void* worker);

protected:
	WorkMode m_workMode;
	ThreadTask* m_task;

private:
	pthread_t m_threadId;
	ThreadPool* m_pool;
};

typedef std::queue<ThreadTask*> ThreadTaskQueue;
typedef std::vector<ThreadWorker*> ThreadWorkerList;
class ThreadPool
{
public:
	ThreadPool(bool autoFreeTask = true);
	~ThreadPool();
	
	bool start();
	void stopAndExit();
	void waitAndExit();

	// should add workers before strat() .
	bool addWorker(ThreadWorker* worker);
	void dismissWorkers();

	/// delete task by worker. so should alloc task at heap memory.
	void addTask(ThreadTask* task);
	ThreadTask* getNextTask();

	inline bool isAutoFreeTaskMode()
	{
		return m_autoFreeTask;
	}

private:
	ThreadWorkerList m_workers;
	ThreadTaskQueue m_taskList;
	pthread_cond_t m_taskReady;
	pthread_mutex_t m_taskListLock;
	pthread_mutex_t m_threadPoolLock;

	bool m_running;
	bool m_nomoreTask;
	bool m_autoFreeTask;
};

#endif

