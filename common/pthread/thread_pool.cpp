#include "thread_pool.h"
#include <cassert>
#include <ctime>
#include <iostream>

void ThreadWorker::waitForComplete()
{
	pthread_join(m_threadId, NULL);
}

bool ThreadWorker::start()
{
	if (m_pool == NULL)
	{
		return false;
	}

	pthread_create(&m_threadId, NULL, ThreadWorker::workLoop, this);
	return true;
}

void ThreadWorker::exit()
{
	pthread_exit(NULL);
}

void* ThreadWorker::workLoop( void* arg )
{
	ThreadWorker* worker = reinterpret_cast<ThreadWorker*>(arg);
	ThreadPool* pool = worker->m_pool;
	worker->beforeWork();
	while(true)
	{
		// wait for next task.
		worker->m_task = pool->getNextTask();
		if (worker->m_task == NULL)
		{
			// nomore task.
			worker->afterWork();
			pthread_exit(NULL);
		}

		worker->doWork();
		if (pool->isAutoFreeTaskMode())
			delete worker->m_task;
		worker->m_task = NULL;
	}

	// not need to reach here
	return NULL;
}

ThreadWorker::~ThreadWorker()
{
	// reduplicate call pthread_join will cause crash.
	//waitForComplete();
}

ThreadPool::ThreadPool(bool autoFreeTask /*= true*/)
	:m_running(false), m_nomoreTask(false), m_autoFreeTask(autoFreeTask)
{
	pthread_cond_init(&m_taskReady, NULL);
	pthread_mutex_init(&m_taskListLock, NULL);
	pthread_mutex_init(&m_threadPoolLock, NULL);
}

ThreadPool::~ThreadPool()
{
	stopAndExit();
	pthread_mutex_destroy(&m_taskListLock);
	pthread_cond_destroy(&m_taskReady);
	pthread_mutex_destroy(&m_threadPoolLock);
	if (isAutoFreeTaskMode())
	{
		while (!m_taskList.empty())
		{
			ThreadTask* task = m_taskList.front();
			m_taskList.pop();
			delete task;
		}
	}
}

bool ThreadPool::addWorker( ThreadWorker* worker )
{
	pthread_mutex_lock(&m_threadPoolLock);
	if (m_running)
	{
		pthread_mutex_unlock(&m_threadPoolLock);
		return false;
	}

	worker->m_pool = this;
	m_workers.push_back(worker);
	pthread_mutex_unlock(&m_threadPoolLock);
	return true;
}

void ThreadPool::dismissWorkers()
{
	stopAndExit();
	pthread_mutex_lock(&m_threadPoolLock);
	m_workers.clear();
	pthread_mutex_unlock(&m_threadPoolLock);
}

void ThreadPool::addTask( ThreadTask* task )
{
	if (task == NULL)
		return;

	pthread_mutex_lock(&m_taskListLock);
	while (m_taskList.size() > 10000)
	{
		pthread_mutex_unlock(&m_taskListLock);
		// avoid use too many memory.
		sleep(1);
		pthread_mutex_lock(&m_taskListLock);
	}
	
	m_taskList.push(task);

	pthread_mutex_unlock(&m_taskListLock);
	pthread_cond_signal(&m_taskReady);
}

ThreadTask* ThreadPool::getNextTask()
{
	ThreadTask* task = NULL;
	pthread_mutex_lock(&m_taskListLock);
	// wait for new task.
	while (m_taskList.empty() && m_running)
	{
		if (m_nomoreTask)
		{
			pthread_mutex_unlock(&m_taskListLock);
			return NULL;
		}
		pthread_cond_wait(&m_taskReady, &m_taskListLock);
	}
	
	if (!m_running)
	{
		pthread_mutex_unlock(&m_taskListLock);
		return NULL;
	}

	assert(!m_taskList.empty());
	task = m_taskList.front();
	m_taskList.pop();
	pthread_mutex_unlock(&m_taskListLock);
	return task;
}

void ThreadPool::stopAndExit()
{
	pthread_mutex_lock(&m_threadPoolLock);
	if (m_running)
	{
		m_running = false;
		pthread_cond_broadcast(&m_taskReady);
		for (size_t i = 0; i < m_workers.size(); i++)
		{
			m_workers[i]->waitForComplete();
		}
	}
	pthread_mutex_unlock(&m_threadPoolLock);
}

void ThreadPool::waitAndExit()
{
	pthread_mutex_lock(&m_threadPoolLock);
	if (!m_nomoreTask && m_running)
	{
		m_nomoreTask = true;
		pthread_cond_broadcast(&m_taskReady);
		for (size_t i = 0; i < m_workers.size(); i++)
		{
			m_workers[i]->waitForComplete();
		}
		m_running = false;
	}
	pthread_mutex_unlock(&m_threadPoolLock);
}

bool ThreadPool::start()
{
	pthread_mutex_lock(&m_threadPoolLock);
	if (m_workers.empty())
	{
		pthread_mutex_unlock(&m_threadPoolLock);
		return false;
	}

	if (!m_running)
	{
		m_running = true;
		for (size_t i = 0; i < m_workers.size(); i++)
		{
			m_workers[i]->start();
		}
	}
	pthread_mutex_unlock(&m_threadPoolLock);
	return true;
}


