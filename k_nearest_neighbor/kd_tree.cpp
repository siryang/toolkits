#include "kd_tree.h"
#include <vector>
#include <algorithm>

using namespace std;

struct Range
{
	int low, high;
}

struct KDTreeBuilderNode
{
	vector<Vector2> datas;

	KDNode* kdNode;
	KDTreeBuilderNode* leftChild;
	KDTreeBuilderNode* rightChild;
}

class KDTreeBuilder
{
public:
	KDTreeBuilder();

	void build(const Vector2* datas, size_t num)
	{
		vector<KDTreeBuilderNode> openNodes;
		KDTreeBuilderNode root;
		root.assign(datas, datas + num);
		openNodes.push_back(root);
		while (!openNodes.empty())
		{
			KDTreeBuilderNode parent = openNodes.back();
			openNodes.pop_back();
			makeSubNode(root);
			if (!root.leftChild != NULL)
				openNodes.push_back(root.leftChild);

			if (!root.rightChild != NULL)
				openNodes.push_back(root.rightChild);
		}
	}

	void makeSubNode(KDTreeBuilderNode root)
	{
		size_t dataNum = root.datas.size();
		sort(root)

		KDNode* kdNode = new KDNode;
		if (dataNum == 1)
		{
			root.leftChild = root.rightChild = NULL;
			kdNode->isLeaf = true;
			kdNode->lChild = kdNode->rChild = NULL;
			kdNode->kv = root.datas[0].x;
		}

		KDNode node;
		node.lChild = node.rChild = NULL;
		node.isLeaf = num == 1;
		if (node.isLeaf)
		{
			node.kv = datas[0].x;
			return node;
		}

		vector<Vector2> sorted;
		sorted.assign(datas, datas + num);
		size_t midIndex = num / 2;
		if (dir == SortDirection_x)
		{
			node.ki = SortDirection_x;
			node.kv = sorted[midIndex].x;
		}
		else
		{
			node.ki = SortDirection_y;
			node.kv = sorted[midIndex].y;
		}
		sort(datas, datas + num, Vector2::lessX);
		sort(datas, datas + num, Vector2::lessY);

		m_left.assign(sorted.begin(), sorted.begin() + midIndex);
		m_right.assign(sorted.begin() + midIndex + 1, sorted.end());
	}

private:
	vector<KDNode> m_nodes;
	Vector2* m_datas;
	size_t m_num;
};

void KDTree::build(Vector2* datas, size_t num)
{
}

