/*
       |----B----C
    A--|
       |----D
*/
#include <cstdlib>
#include <iostream>

using namespace std;

class A
{
public:
	virtual void show() {cout << "show a" << endl;}
	virtual bool isDataValid() { return false; }
	virtual size_t getDataSize() const  = 0;
	virtual int parsedZoomLevel() { return -1; };
	virtual bool parsedForNightMode() { return false; }
};

class B: public A
{
public:
	//virtual void show() {cout << "show b" << endl;}
	
	virtual void getModelDots(int modelDots) = 0;
	virtual void drawGL(int par) = 0;
	virtual void getDots(int dots) = 0;
	virtual void getRoadDots(int roadDots) = 0;
	virtual void parseForZoomLevel(int m) = 0;

	virtual size_t getDataSize() const { return 0; }
	virtual int parsedZoomLevel() const { return 1; }
	virtual bool parsedForNightMode() const { return true; }
};

class C: public B
{
public:
	//virtual void show() { cout << "show c" << endl;}
	virtual void parseForZoomLevel(int m){ cout << m;}
	virtual void drawGL(int par) {cout << par;}
	virtual void getDots(int dots){dots;}
	virtual void getRoadDots(int roadDots){roadDots;}
	virtual void getModelDots(int modelDots){modelDots;}
	virtual bool isDataValid() { return true; }


};

class D: public A
{
public:
	virtual void show() { cout << "show d" << endl;}

	virtual bool isDataValid() { return 1; }
	virtual size_t getDataSize() const { return 2; };
	virtual int parsedZoomLevel() { 
		return -1; 
	};
	virtual bool parsedForNightMode() { return false; }
};

struct TestCallback
{
	void (*Callback)(int p);
};

#include "inherit_diff_namespace.h"
void testInheritDiffNamespace()
{
	parent::Node* child = new child::Node();
	child->show();
	delete child;
	
	
}

void main()
{
	testInheritDiffNamespace();
	A* pointer = new C();
	pointer->show();
	system("pause");
}
