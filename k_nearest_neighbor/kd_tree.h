#pragma once
#include <deque>

struct Vector3
{
	int x, y, z;

	typedef bool LessFunction(const Vector3&, const Vector3&);
	static LessFunction* getLessFunction(int dimension)
	{
		switch (dimension)
		{
		case 0:
			return less < 0 > ;
		case 1:
			return less < 1 > ;
		case 2:
			return less < 2 > ;
		default:
			return NULL;
		}
	}
	
	template<int>
	static bool less(const Vector3& l, const Vector3& r);

	template<>
	static bool less<0>(const Vector3& l, const Vector3& r)
	{ return l.x < r.x; }

	template<>
	static bool less<1>(const Vector3& l, const Vector3& r)
	{ return l.y < r.y; }

	template<>
	static bool less<2>(const Vector3& l, const Vector3& r)
	{
		return l.z < r.z;
	}
};

struct KDNode
{
	KDNode* lChild;
	KDNode* rChild;
	Vector3 value;
	int dimension;
	int index;
	//bool isLeaf;
};

class KDTree
{
public:
	~KDTree();

	void build(Vector3* datas, size_t num);

	size_t matchBest(Vector3 value);
private:

private:
	KDNode* m_root;
};