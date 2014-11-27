#include "oh_geometry.h"
#include "oh_type.h"
#include <vector>
using namespace std;

class Polygon
{
	// a --> b --> c is ccw
	// AB x AC == |AB| * |AC| * sin¡ÏCAB
	// if (ab x ac > 0) // means CAB < 180
	//		return ccw
	// else IF ab x ac == 0  // means CAB == 180
	//		return ...?
	// else return cw // means CAB >= 180
	public static int ccw(Point a, Point b, Point c) {
		return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
	}

	// area = 
	unsigned int getArea(const Point* polygon, size_t pointNum)
	{
		unsigned int area = 0;
		const Point& A = polygon[0];
		for (size_t i = 2; i < pointNum; i++)
		{
			const Point& B = polygon[i - 1];
			const Point& C = polygon[i];
			// get Area ABC
			int x1 = A.x - B.x;
			int y1 = A.y - B.y;
			int x2 = B.x - C.x;
			int y2 = B.y - C.y;

			int v = (x1 * y2 - x2 * y1);
			area += abs(v) / 2;
		}
		return area;
	}

	//  Public-domain function by Darel Rex Finley, 2006.
	double polygonArea(PointF* contour, size_t num) 
	{
		float A = 0.0f;

		for(size_t p = num-1, q = 0; q < num; p = q++)
		{
			A += contour[p].x * contour[q].y - contour[q].x * contour[p].y;
		}
		return A * 0.5f;
	}

	bool isClockWise(vector<Point>&points)
	{
		const Point* contour = &points[0];
		size_t n = points.size();
		double A = 0.0;

		for(size_t p = n-1, q = 0; q < n; p = q++)
		{
			A += (double)contour[p].x * (double)contour[q].y - (double)contour[q].x * (double)contour[p].y;
		}
		return A < 0.0;
	}

};


class Line
{
	Point start, end;

	//min(xi,xj) <= xk <= max(xi,xj) and min(yi,yj) <= yk <= max(yi,yj)
};
