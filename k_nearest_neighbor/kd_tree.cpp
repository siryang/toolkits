#include "kd_tree.h"
#include <vector>
#include <algorithm>
#include <list>

using namespace std;

template<typename ValueType>
class KDTreeBuilder
{
	struct NodeValueType
	{
		ValueType value;
		int index;
	};

	struct KDTreeBuildNode
	{
		KDNode* kdNode;
		KDTreeBuildNode* leftChild;
		KDTreeBuildNode* rightChild;
		int deep;

		vector<NodeValueType> datas;
	};

public:
	KDTreeBuilder(){}

	KDNode* build(const ValueType* datas, size_t num)
	{
		m_nodes.reserve(num);

		m_nodes.push_back(KDTreeBuildNode());
		KDTreeBuildNode* root = &m_nodes[0];
		root->kdNode = new KDNode();
		root->datas.resize(num);
		root->deep = 0;
		for (size_t i = 0; i < num; i++)
		{
			root->datas[i].value = datas[i];
			root->datas[i].index = i;
		}

		vector<KDTreeBuildNode*> openNodes;
		openNodes.push_back(root);
		while (!openNodes.empty())
		{
			KDTreeBuildNode* node = openNodes.back();
			openNodes.pop_back();
			makeSubNode(node);
			if (node->leftChild != NULL)
				openNodes.push_back(node->leftChild);

			if (node->rightChild != NULL)
				openNodes.push_back(node->rightChild);
		}
		return root->kdNode;
	}

	void makeSubNode(KDTreeBuildNode* node)
	{
		KDNode* kdNode = node->kdNode;
		size_t dataNum = node->datas.size();
		if (dataNum == 1)
		{
			//kdNode->isLeaf = true;
			node->leftChild = node->rightChild = NULL;
			kdNode->lChild = kdNode->rChild = NULL;
			kdNode->value = node->datas[0].value;
			kdNode->index = node->datas[0].index;
			vector<NodeValueType>().swap(node->datas);
			return;
		}

		vector<NodeValueType>& datas = node->datas;

		kdNode->dimension = node->deep % 2;
		int dimension = kdNode->dimension;
		sort(node->datas.begin(), node->datas.end()
			, [dimension](const NodeValueType& l, const NodeValueType& r){
			ValueType::LessFunction* lessFunction = ValueType::getLessFunction(dimension);
			return lessFunction(l.value, r.value);
		});

		size_t midIndex = dataNum / 2;
		kdNode->value = datas[midIndex].value;
		kdNode->index = datas[midIndex].index;
		//kdNode->isLeaf = false;

		if ( midIndex >= 1 )
		{
			m_nodes.push_back(KDTreeBuildNode());
			KDTreeBuildNode* leftChild = &m_nodes[m_nodes.size() - 1];
			leftChild->deep = node->deep + 1;
			leftChild->datas.assign(&datas[0], &datas[0] + midIndex);
			leftChild->kdNode = new KDNode();

			node->leftChild = leftChild; 
			kdNode->lChild = leftChild->kdNode;
		}
		else
		{
			node->leftChild = NULL;
			kdNode->lChild = NULL;
		}

		if ( midIndex <= dataNum - 2 )
		{
			m_nodes.push_back(KDTreeBuildNode());
			KDTreeBuildNode* rightChild = &m_nodes[m_nodes.size() - 1];
			rightChild->deep = node->deep + 1;
			rightChild->datas.assign(&datas[0] + midIndex + 1, &datas[0] + dataNum);
			rightChild->kdNode = new KDNode();

			kdNode->rChild = rightChild->kdNode;
			node->rightChild = rightChild;
		}
		else
		{
			node->rightChild = NULL;
			kdNode->rChild = NULL;
		}
		vector<NodeValueType>().swap(node->datas);
	}

private:
	vector<KDTreeBuildNode> m_nodes;
};


void KDTree::build(Vector3* datas, size_t num)
{
	KDTreeBuilder<Vector3> builder;
	m_root = builder.build(datas, num);
}

KDTree::~KDTree()
{
	list<KDNode*> nodes;
	nodes.push_back(m_root);
	while (!nodes.empty())
	{
		KDNode* node = nodes.back();
		nodes.pop_back();
		if (m_root->lChild != NULL)
			nodes.push_back(m_root->lChild);

		if (m_root->rChild != NULL)
			nodes.push_back(m_root->rChild);
		delete node;
	}
}

size_t KDTree::matchBest(Vector3 value)
{
}
