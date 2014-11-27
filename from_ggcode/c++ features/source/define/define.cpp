#include <typeinfo>
#include <iostream>
#include <cstdlib>

using namespace std;

struct SegmentPriority
{
	int a;
	int b;
};

void typeid_test()
{
	cout << typeid(int).name() << endl; 
	cout << typeid(double).name() << endl; 
	cout << typeid(char).name() << endl; 
	cout << typeid(SegmentPriority).name() << endl; 
	system("pause");
}

int main()
{
	return 0;
}
