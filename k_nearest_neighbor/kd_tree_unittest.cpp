#include "kd_tree.h"
#include "gtest/gtest.h"

#define element_of(a) (sizeof(a) / sizeof((a)[0]))

TEST(KDTree, build)
{
	Vector3 datas[] = {
		{ 7, 2, 1 },
		{ 5, 4, 1 },
		{ 2, 3, 1 },
		{ 4, 7, 1 },
		{ 8, 1, 1 },
		{ 9, 6, 1 },
	};

	KDTree* kdTree = new KDTree();
	kdTree->build(&datas[0], element_of(datas));
}