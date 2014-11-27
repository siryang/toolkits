#include "common_header.h"
#include <iostream>

void testPointer(Point** pos, size_t num)
{
	for (size_t i = 0; i < num; i++)
	{
		cout << "(" << pos[i]->x << "," << pos[i]->y << ")" << endl;
	}

	// ������� new �������ڴ治��������(*pos)����ƶ�i*sizeof(Point)�ͻ���ʵ��Ƿ��ռ䡣
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

