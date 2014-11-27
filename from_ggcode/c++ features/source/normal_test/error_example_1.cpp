#include "common_header.h"
#include <iostream>

void testPointer(Point** pos, size_t num)
{
	for (size_t i = 0; i < num; i++)
	{
		cout << "(" << pos[i]->x << "," << pos[i]->y << ")" << endl;
	}

	// 如果连续 new 出来的内存不连续，则(*pos)向后移动i*sizeof(Point)就会访问到非法空间。
	for (size_t i = 0; i < num; i++)
	{
		cout << "(" << (*pos)[i].x << "," << (*pos)[i].y << ")" << endl;
	}
}

int error_example1_main()
{
	Point *n[3];
	for (size_t i = 0; i < 3; i++)
	{
		n[i] = (Point*) malloc (sizeof(Point) * 2);
		n[i]->x = 0;
		n[i]->y = 0;
	}

	testPointer(n, 3);

	for each(Point* p in n)
	{
		free(p);
	}
	return 0;
}

