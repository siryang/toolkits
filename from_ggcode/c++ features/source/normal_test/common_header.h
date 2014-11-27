#pragma once
#include <cstdlib>
#include <iostream>

using namespace std;

struct Point
{
	int x, y;
	Point():x(0), y(0){}
	Point(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
};

int error_example1_main();
void error_example_2_main();

