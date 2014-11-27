#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

void testFunc(int a, int b)
{
	printf ("%d, %d\n", a, b);
}

#define testFunc(a) testFunc(a, 0)

typedef enum SegmentPriority
{
	this_is_0 = 0,
	this_is_1 = 1,
} SegmentPriority;

void testEnum()
{
	SegmentPriority p = (SegmentPriority)-2;
	if (p < this_is_0)
	{
		printf ("Smaller than 0\n");
	}
	else
	{
		printf ("Bigger than 0\n");
	}
}

void printDataTime(LPSYSTEMTIME time)
{
	printf("%4d-%2d-%2d %2d:%2d:%2d:%3d\n"
		, time->wYear, time->wMonth, time->wDay, time->wHour, time->wMinute, time->wSecond, time->wMilliseconds);
}

int main()
{
	double m = 3.3;
	double n = 2.2;

	char testSrc[] = "Hello wORLD";
	char* tempStr = (char*) malloc (strlen(testSrc) + 1);
	char* dest = tempStr;

	SYSTEMTIME time1, time2;
	GetSystemTime(&time1);
	GetLocalTime(&time2);
	printDataTime(&time1);
	printDataTime(&time2);



	testEnum();

	strncpy(dest, testSrc, strlen(testSrc));
	dest += strlen(testSrc);
	*dest = 0;
	printf (dest);

	free(tempStr);


	printf("%f\n%f\n", m, n);

	exit(0);
	printf("Hi");
	testFunc(1);
	testFunc(1, 2);
	return 0;
}

