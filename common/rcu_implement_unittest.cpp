#include <gtest/gtest.h>
#include "rcu_implement.h"
#include <iostream>

using namespace std;

class DataNode: public RcuNode
{
public:
	DataNode(){
		m_nodeKey++;
		m_nodeNumber++;
		cout << "construct:" << m_nodeKey << endl;
	}

	virtual ~DataNode(){
		m_nodeNumber--;
		cout << "destruct:" << m_nodeKey << endl; 
	}

	static int getNodeNumber()
	{
		return m_nodeNumber;
	}

private:
	static int m_nodeNumber;
	static int m_nodeKey;
};

int DataNode::m_nodeNumber = 0;
int DataNode::m_nodeKey = 0;


TEST(RcuImplement, testSingleThread)
{
	RcuImplement* manager = new RcuImplement;

	const RcuNode* nodeNull = manager->p();
	EXPECT_TRUE(nodeNull == NULL);

	DataNode* data = new DataNode;;
	manager->addData(data);

	const DataNode* postData1 = (const DataNode*)manager->p();
	EXPECT_EQ(postData1, data);

	DataNode* data2 = new DataNode;;
	manager->addData(data2);
	const DataNode* postData2 = (const DataNode*)manager->p();
	EXPECT_EQ(postData2, data2);

	manager->v(postData1);
	manager->v(postData2);
	EXPECT_EQ(DataNode::getNodeNumber(), 1);

	delete manager;
	EXPECT_EQ(DataNode::getNodeNumber(), 0);
}


