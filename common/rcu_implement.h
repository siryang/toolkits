#pragma once

#include <vector>
extern "C"
{
#include "pthread/pthread.h"
#include "pthread/semaphore.h"
};

/**
simple:
class Data
:public BaseNodeData
{
public:
void release(){};
}
BaseNodeData* p = new Data;
DataReadUpdateManager dataManager;
dataManager.update(p);

Data* data = reinterpret_cast<Data*>(dataManager.p());
...
dataManager.v(reinterpret_cast<BaseData*>data);

DataReadUpdateManager can help you release memory of BaseNodeData.
*/


class RcuNode
{
public:
	virtual ~RcuNode(){}
};

class RcuImplement
{
public:
	RcuImplement()
	{
		pthread_rwlock_init(&m_queueLock, NULL);
	}

	~RcuImplement();

	struct Node
	{
		sem_t readerNum;
		const RcuNode* data;

		Node(const RcuNode* _data)
		{
			data = _data;
			sem_init(&readerNum, 0, 0);
		}

		~Node()
		{
			delete data;
			sem_destroy(&readerNum);
		}
	};

	static inline bool equal(const RcuNode* left, const Node* right)
	{
		return left == right->data;
	}

	// passeren
	const RcuNode* p();

	// vrijgeven
	void v(const RcuNode* data);

	void addData(const RcuNode* data);

private:
	std::vector<Node*> m_queue;
	pthread_rwlock_t m_queueLock;
};