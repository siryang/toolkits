#include <iostream>
#include "../lcs/lcs.h"
#include "oh_type.h"

using namespace std;

struct Line
{
	Point start, end;
};
// 判断两线段是否相交
#define LINE_TO_VECTOR(line, vecX, vecY) {\
	*(vecX) = (line)->end.x - (line)->start.x;\
	*(vecY) = (line)->end.y - (line)->start.y;\
}

#define VECTOR_MULTI(vecX1, vecY1, vecX2, vecY2) ((vecX1) * (vecY2) - (vecX2) * (vecY1))

bool isLineIntersect(const Line& line1, const Line& line2)
{
	int vecX1, vecY1, vecX2, vecY2;
	LINE_TO_VECTOR(&line1, &vecX1, &vecY1);
	LINE_TO_VECTOR(&line2, &vecX2, &vecY2);
	
	int vecStartX, vecStartY, vecEndX, vecEndY;
	vecStartX = line2.start.x - line1.start.x;
	vecStartY = line2.start.y - line1.start.y;
	vecEndX = line2.end.x - line1.start.x;
	vecEndY = line2.end.y - line1.start.y;

	// 符号相反，则有交点。
	// 为0，则重合
	if ((VECTOR_MULTI(vecX1, vecY1, vecStartX, vecStartY) > 0)
		^ (VECTOR_MULTI(vecX1, vecY1, vecEndX, vecEndY) > 0))
		return false;

	vecStartX = -vecStartX;
	vecStartY = -vecStartY;
	vecEndX = line1.end.x - line2.start.y;
	vecEndY = line1.end.y - line2.start.y;

	if ((VECTOR_MULTI(vecX2, vecY2, vecStartX, vecStartY) > 0)
		^ (VECTOR_MULTI(vecX2, vecY2, vecEndX, vecEndY) > 0))
		return false;

	return true;
}

bool isLineIntersectWithPolygon(const Point* polygon, size_t pointNum, const Line& line)
{
	for (size_t i = 1; i < pointNum; i++)
	{
		Line temp;
		temp.start = polygon[i];
		temp.end = polygon[i - 1];
		if (isLineIntersect(temp, line))
			return true;
	}
	return false;
}

void main()
{
	cout << "Hello world" << endl;

	string lcs;
	lcs = getLCS("12253302", "153");
	cout << lcs << endl;

	lcs = getLCS("12253302", "152253000");
	cout << lcs << endl;
}
