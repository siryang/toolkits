#include "thread_monitor.h"


#if defined(WIN32)
#include <Windows.h>
#endif

#ifndef FETCH_TMC_INTERVAL
#define FETCH_TMC_INTERVAL 5
#endif


#if defined(linux) && defined(USE_INOTIFY)
#include <linux/unistd.h>
#include <sys/select.h>
#include <sys/inotify.h>
#include <cstring>
/*
int inotify_init(int flag)
{
	return syscall(__NR_inotify_init1, flag);
}

int inotify_add_watch(int fd, const char* path, unsigned int mask)
{
	return syscall(__NR_inotify_add_watch, fd,  path, mask);
}

int inotify_rm_watch(int fd, unsigned int mask)
{
	return syscall(__NR_inotify_rm_watch, fd, mask);
}

*/
#endif

bool MonitorThread::eventHandle()
{
	// should exit?
	pthread_testcancel();
	pthread_mutex_lock(&m_switchOnOffLock);
	if (!m_on)
	{
		pthread_mutex_unlock(&m_switchOnOffLock);
		pthread_exit(NULL);
		return false;
	}
	pthread_mutex_unlock(&m_switchOnOffLock);

	// handle events
	pthread_mutex_lock(&m_evetListLock);
	pthread_mutex_lock(&m_listenerLock);
	if (!m_eventList.empty())
	{
		int* event = &m_eventList[0];
		int* eventEnd = event + m_eventList.size();
		for (ListenerList::iterator iterListener = m_listerers.begin();
			iterListener != m_listerers.end(); iterListener++)
		{
			ThreadEventListener& listener = *iterListener;
			for (; event != eventEnd; event++)
			{
				if (listener.event == *event)
				{
					listener.proc(listener.param);
				}
			}
		}
		m_eventList.clear();
	}
	pthread_mutex_unlock(&m_listenerLock);
	pthread_mutex_unlock(&m_evetListLock);
	return true;
}

// 监视文件修改
void* MonitorThread::threadFunction(void* pm)
{
	MonitorThread* monitor = reinterpret_cast<MonitorThread*>(pm);
	while(true)
	{
		monitor->m_condLock.wait();
		if (!monitor->eventHandle())
		{
			return NULL;
		}
	}
}

void MonitorThread::registe( int event, ThreadEventProc proc, void* param )
{
	ThreadEventListener listener;
	listener.event = event;
	listener.proc = proc;
	listener.param = param;
	pthread_mutex_lock(&m_listenerLock);
	m_listerers.push_back(listener);
	pthread_mutex_unlock(&m_listenerLock);
}

void MonitorThread::unregiste( int event, ThreadEventProc proc )
{
	pthread_mutex_lock(&m_listenerLock);
	for (ListenerList::iterator iterListener = m_listerers.begin();
		iterListener != m_listerers.end();)
	{
		ThreadEventListener& listener = *iterListener;
		if (listener.event == event && listener.proc == proc){
			iterListener = m_listerers.erase(iterListener);
		}
		else{
			iterListener++;
		}
	}
	pthread_mutex_unlock(&m_listenerLock);
}

void MonitorThread::raise( int event )
{
	pthread_mutex_lock(&m_evetListLock);
	m_eventList.push_back(event);
	pthread_mutex_unlock(&m_evetListLock);
	m_condLock.signal();
}


