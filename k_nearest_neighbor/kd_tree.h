#pragma once
#include <deque>

struct Vector2
{
	float x, y;
	
	static bool lessX(const Vector2& l, const Vector2& r)
	{ return l.x < r.x; }

	static bool lessY(const Vector2& l, const Vector2& r)
	{ return l.x < r.x; }
};

enum SortDirection
{
	SortDirection_x,
	SortDirection_y
};

struct KDNode
{
	KDNode* lChild;
	KDNode* rChild;
	SortDirection ki;			// ��Node������������
	float kv;		/**< partition key value *///ֱ��ͼ�����������ϵ�������м�ģֵ  
	bool isLeaf;                    /**< 1 if node is a leaf, 0 otherwise */
	//struct feature* features;    /**< features at this node */
	//int n;                       /**< number of features */
};

class KDTree
{
public:
	~KDTree();

	void build(Vector2* datas, size_t num);

	// always insert as left node.
	void insert();

	void remove();

private:
	void addNode();

private:
	KDNode* m_root;
};