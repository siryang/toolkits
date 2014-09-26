#include "rcu_implement.h"

#include <functional>
#include <algorithm>

using namespace std;

RcuImplement::~RcuImplement()
{
	pthread_rwlock_wrlock(&m_queueLock);
	size_t nodeNum = m_queue.size();
	for (size_t i = 0; i < nodeNum; i++)
	{
		delete m_queue[i];
	}
	pthread_rwlock_unlock(&m_queueLock);

	pthread_rwlock_destroy(&m_queueLock);
}


void RcuImplement::v(const RcuNode* data)
{
	bool isNewestNode = false;
	std::vector<Node*>::iterator iter;

	// find data in list.
	pthread_rwlock_rdlock(&m_queueLock);

	iter = find_if(m_queue.begin(), m_queue.end(), bind1st(ptr_fun(RcuImplement::equal), data));
	if (iter == m_queue.end())
	{
		pthread_rwlock_unlock(&m_queueLock);
		return;
	}

	isNewestNode = (iter == m_queue.end() - 1);

	Node* node = *iter;
	sem_trywait(&node->readerNum); // reader num --
	int readerNum;
	sem_getvalue(&node->readerNum, &readerNum);
	//cout << (size_t)data << ", readerNum: --" << readerNum << endl;
	//cout << "			v data, now node number: " << m_queue.size() << endl
	//<< "reader number: " << readerNum << endl;

	if (!isNewestNode && readerNum == 0)
	{
		delete node;
		m_queue.erase(iter);
		//cout << "data node number" << m_queue.size() << endl;
	}
	pthread_rwlock_unlock(&m_queueLock);

}

const RcuNode* RcuImplement::p()
{
	pthread_rwlock_rdlock(&m_queueLock);
	if (m_queue.empty())
	{
		pthread_rwlock_unlock(&m_queueLock);
		return NULL;
	}

	Node* node = m_queue.back();
	pthread_rwlock_unlock(&m_queueLock);

	sem_post(&node->readerNum);

	//int readerNum;
	//sem_getvalue(&node->readerNum, &readerNum);
	//cout << (size_t)node->data << ", readerNum: ++" << readerNum << endl;

	return node->data;
}

void RcuImplement::addData(const RcuNode* data)
{
	Node* node = new Node(data);
	pthread_rwlock_wrlock(&m_queueLock);
	if (!m_queue.empty())
	{
		Node* prevNode = m_queue.back();
		int prevDataReaderNum = 0;
		sem_getvalue(&prevNode->readerNum, &prevDataReaderNum);
		if (prevDataReaderNum == 0)
		{
			m_queue.back() = node;
			//cout << "delete : " << (size_t)data << endl;
			//cout << "data copy num:" << m_queue.size() << endl;
			// release mutex befor delete prevNode. delete cost to much time.
			pthread_rwlock_unlock(&m_queueLock);
			delete prevNode;
			return;
		}
	}

	m_queue.push_back(node);
	//cout << "data copy num:" << m_queue.size() << endl;
	pthread_rwlock_unlock(&m_queueLock);
}
