#include "save_princess2.h"
#include "gtest/gtest.h"
using namespace SavePrincess;

TEST(SavePrincess, case) 
{
	int n, r, c;
#define USE_TEST_CASE
#if defined(USE_TEST_CASE)
	n = 5;
	r = 3;
	c = 1;

	char grid[101][101] = {
		{"-----"},
		{"-----"},
		{"-----"},
		{"-m--p"},
		{"-----"},
	};
#else
	char grid[101][101];
	scanf("%d", &n);
	scanf("%d", &r);
	scanf("%d", &c);

	for(int i=0; i<n; i++) {
		scanf("%s[^\n]%*c", grid[i]);
	}
#endif
	nextMove(n, r, c, grid);
}