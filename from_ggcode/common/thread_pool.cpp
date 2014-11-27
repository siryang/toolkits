#include "thread_pool.h"
#include <cassert>
#include <ctime>
#include <iostream>
#include <windows.h>

#ifndef sleep
#define sleep(a) Sleep((a) * 1000)
#endif

void ThreadWorker::waitForComplete()
{
	pthread_join(m_threadId, NULL);
}

void ThreadWorker::start()
{
	pthread_create(&m_threadId, NULL, ThreadWorker::workLoop, this);
}

void ThreadWorker::exit()
{
	pthread_exit(NULL);
}

void* ThreadWorker::workLoop( void* arg )
{
	ThreadWorker* worker = (ThreadWorker*)arg;
	ThreadPool* pool = worker->m_pool;
	while(pool->isRunning())
	{
		//while (worker->m_task == NULL)
		{
			// wait for next task.
			worker->m_task = pool->getNextTask();
			if (worker->m_task == NULL)
			{
				pthread_exit(NULL);
			}
		}

		worker->doTask();
		// need new task?
		if (worker->m_workMode != WorkMode_loop)
		{
			delete worker->m_task;
			worker->m_task = NULL;
		}
		pthread_testcancel();
	}

	return NULL;
}


ThreadPool::ThreadPool()
	:m_running(false)
{
	pthread_mutex_init(&m_taskListLock, NULL);
	pthread_cond_init(&m_taskReady, NULL);
}

ThreadPool::~ThreadPool()
{
	m_running = false;
	pthread_mutex_destroy(&m_taskListLock);
	pthread_cond_destroy(&m_taskReady);
}

bool ThreadPool::setWorkers( ThreadWorker* worker, size_t num )
{
	if (m_workers == NULL)
	{
		m_workers = worker;
		m_workersNum = num;
		return true;
	}
	return false;
}

ThreadWorker* ThreadPool::dismissWorkers()
{
	exit();

	ThreadWorker* temp;
	temp = m_workers;
	m_workers = NULL;
	m_workersNum = 0;
	return temp;
}

void ThreadPool::addTask( ThreadTask* task )
{
	if (task == NULL)
		return;

	pthread_mutex_lock(&m_taskListLock);
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

void ThreadPool::exit()
{
	if (m_running)
	{
		m_running = false;
		for (size_t i = 0; i < m_workersNum; i++)
		{
			m_workers[i].waitForComplete();
		}
	}
}

void ThreadPool::waitForAllTaskComplete()
{
	if (!m_nomoreTask && m_running)
	{
		m_nomoreTask = true;
		pthread_cond_broadcast(&m_taskReady);
		for (size_t i = 0; i < m_workersNum; i++)
		{
			m_workers[i].waitForComplete();
		}
	}
}

bool ThreadPool::start()
{
	if (m_workersNum == 0)
		return false;

	if (!m_running)
	{
		m_running = true;
		for (size_t i = 0; i < m_workersNum; i++)
		{
			m_workers[i].start();
		}
	}
	return true;
}


