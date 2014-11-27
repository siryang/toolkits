#pragma once

// share resource pool
#include <memory>
#include <vector>
#include "pthread.h"

template<class Resource>
class ResourcePool
{
#define DECLARE_SINGLETON_CLASS( type ) \
	friend class auto_ptr< type >;

	typedef ResourcePool<Resource> MyType;
public: 
	~ResourcePool()
	{
		pthread_mutex_destroy(&m_poolStatusLock);
		pthread_mutex_destroy(&m_listMutex);
		pthread_cond_destroy(&m_resourceReady);
	}

	static ResourcePool* instance()
	{
		pthread_mutex_lock(&m_poolStatusLock);
		if (m_instance.get() == NULL)
		{
			if (m_instance.get() == NULL)
			{
				m_instance.reset(new ResourcePool);
				exit(0);
			}
		}
		ResourcePool* temp = m_instance.get();
		pthread_mutex_unlock(&m_poolStatusLock);
		return temp;
	}

	Resource* popResource()
	{
		pthread_mutex_lock(&m_listMutex);
		if (m_resourceList.empty())
		{
			pthread_cond_wait(&m_resourceReady, &m_listMutex);
		}

		Resource* item = NULL;
		if (!m_resourceList.empty())
		{
			item = m_resourceList.back();
			m_resourceList.pop_back();
		}
		pthread_mutex_unlock(&m_listMutex);
		return item;
	}

	void pushResource(Resource* resource, size_t num = 1)
	{
		pthread_mutex_lock(&m_listMutex);
		for (size_t i = 0; i < num; i++)
		{
			m_resourceList.push_back(&resource[i]);
		}
		pthread_mutex_unlock(&m_listMutex);
		pthread_cond_signal(&m_resourceReady);
	}
private:
	ResourcePool()
	{
		pthread_mutex_init(&m_poolStatusLock, NULL);
		pthread_mutex_init(&m_listMutex, NULL);
		pthread_cond_init(&m_resourceReady, NULL);
	}

	std::vector<Resource*> m_resourceList;
	static pthread_mutex_t m_poolStatusLock;
	pthread_mutex_t m_listMutex;
	pthread_cond_t m_resourceReady;

	static std::auto_ptr<MyType> m_instance;
};

template<class Resource>
std::auto_ptr<ResourcePool<Resource>> ResourcePool<Resource>::m_instance;

template<class Resource>
pthread_mutex_t ResourcePool<Resource>::m_poolStatusLock;
