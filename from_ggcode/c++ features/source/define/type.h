#include <cstdio>
struct Position
{
#ifdef PosInt
	int x, y;
#else
	double x, y;
#endif
};

int getInfo(Position *p)
{

#if defined(PosInt)
	int m = p->x + p->y;
	printf ("result int : %d", m);
#else ifdef(PosDouble)
	double m = p->x + p->y;
	printf ("result double: %f", m);
#endif
	return 0;
}
