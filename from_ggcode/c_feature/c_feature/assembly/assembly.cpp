// assembly.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

typedef int (*p)();
int _tmain();

p test = _tmain;

int cmp(int a)
{
	a = a << 1;
	return 1;
}

void testMyLed()
{
	printf ("This is a test function of my testing...");
}

struct Point
{
	int x, y;
};


typedef __int64 ObjectId;
typedef char BYTE;
enum SegmentDirection{sdsf2};
enum CrossWalkType{sdsf};
enum TrafficSide{sdf};
enum SegmSlopeSource{dd};
enum SegmentPriority{xxx = 15};
enum SegmentUsage{xxxxx = 15};
enum OverpassType{xxxx};
enum SlopeHint{ssss};
typedef unsigned int uint32;
typedef unsigned short uint16;
struct SegmSource
{
	// 25 item
	ObjectId segmId;
	ObjectId sNodeId;
	ObjectId eNodeId;

	int nMapId;
	uint32 pointNum;
	Point* points;
	uint16 ui16Length;

	SegmentPriority bLevel;			
	SegmentUsage bUsage;	//SegmentUsage
	// fixit : use SegmentDirection replace bDirection;
	SegmentDirection bDirection;	// 1 : 双向 2：顺向 3：逆向
	BYTE bFuncClass;
	BYTE oriFuncClass;
	BYTE bWidthLevel;

	bool bToll;
	bool boHasName;
	bool pedestrianEnabled;
	bool sideRoad;
	bool tunnel;

	CrossWalkType crossWalkType;
	TrafficSide trafficSide;
	OverpassType overpassType;
	SegmSlopeSource slopeSource;

	SlopeHint sNodeSlope;
	SlopeHint eNodeSlope;

	const wchar_t* segName;

	void clear(){}

	ObjectId getOtherNode(ObjectId nodeId)
	{
		return nodeId == sNodeId? eNodeId: sNodeId;
	}

};

void structLength()
{
	cout << "SegmentPriority	" << sizeof(SegmentPriority) << endl
		<< "SegmentUsage	" << sizeof(SegmentUsage) << endl
		<< "CrossWalkType	" << sizeof(CrossWalkType) << endl
		<< "bool	" << sizeof(bool) << endl
		<< "SegmSource	" << sizeof(SegmSource) << endl;


	TempTest<int>::instance()->show();
	TempTest<Point>::instance()->show();
	cmp(3);
}

#include <iostream>
using namespace std;

void runCmd(const char* cmd)
{
	FILE* fp = _popen(cmd, "r");
	fclose(fp);
}

int _tmain()
{
	runCmd("red_splitter.exe");
	return 0;
}

