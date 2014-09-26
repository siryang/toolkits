#pragma once
#include "pthread/pthread.h"

class ConditionLock
{
public:
	ConditionLock()
	{
		pthread_mutex_init(&m_mutex, NULL);
		pthread_cond_init(&m_cond, NULL);
	}

	~ConditionLock()
	{
		pthread_mutex_destroy(&m_mutex);
		pthread_cond_destroy(&m_cond);
	}

	void lock()
	{
		pthread_mutex_lock(&m_mutex);
	}

	void unlock()
	{
		pthread_mutex_unlock(&m_mutex);
	}

	// lock(); wait(); unlock();
	void wait()
	{
		pthread_cond_wait(&m_cond, &m_mutex);
	}

	// lock(); wait(); unlock();
	void boardcast()
	{
		pthread_cond_broadcast(&m_cond);
	}
private:
	pthread_mutex_t m_mutex;
	pthread_cond_t m_cond;
};


