#ifndef NAVI_ONLINE_NETNAVI_DATA_MANAGER_H_
#define NAVI_ONLINE_NETNAVI_DATA_MANAGER_H_
#include <vector>
#include <string>
#include <map>

#include "pthread/pthread.h"
#include "condition_lock.h"

using namespace std;

typedef void(*ThreadEventProc)(void*);

struct ThreadEventListener
{
	int event;
	ThreadEventProc proc;
	void* param;
};

class MonitorThread
{
public:
	typedef vector<ThreadEventListener> ListenerList;

	static void* threadFunction(void* pm);

	void registe(int event, ThreadEventProc proc, void* param);

	void unregiste(int event, ThreadEventProc proc);
	
	void raise(int event);
	
	bool eventHandle();

	inline void on()
	{
		if (!m_on)
		{
			m_on = true;
			pthread_create(&m_monitorThradId, NULL, MonitorThread::threadFunction, this);
			//cout << "listen thread, id :" << m_monitorThradId.x << "," << (size_t)m_monitorThradId.p << endl;;
		}
	}

	inline void off()
	{
		if (m_on)
		{
			pthread_mutex_lock(&m_switchOnOffLock);
			m_on = false;
			pthread_mutex_unlock(&m_switchOnOffLock);
			pthread_cancel(m_monitorThradId);
			pthread_join(m_monitorThradId, NULL);
		}
	}

	MonitorThread()
	{
		pthread_mutex_init(&m_switchOnOffLock, NULL);
		pthread_mutex_init(&m_listenerLock, NULL);
		pthread_mutex_init(&m_evetListLock, NULL);
	}

	~MonitorThread()
	{
		off();
		m_condLock.boardcast();
		pthread_mutex_destroy(&m_switchOnOffLock);
		pthread_mutex_destroy(&m_listenerLock);
		pthread_mutex_destroy(&m_evetListLock);
	}
private:
	pthread_t m_monitorThradId;
	ConditionLock m_condLock;

	bool m_on;
	pthread_mutex_t m_switchOnOffLock;

	ListenerList m_listerers;
	pthread_mutex_t m_listenerLock;

	vector<int> m_eventList;
	pthread_mutex_t m_evetListLock;
};

#endif

