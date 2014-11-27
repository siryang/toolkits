#include "stdafx.h"
#include "../example_algorithm.h"
#include "../example_container.h"

//namespace test_algorithm
//{
//}

static bool elem_equal(const Elem& elem)
{
	return elem.key == 2;
}

struct ElemCmp
{
private:
	int m_search;
public:
	ElemCmp(int key):m_search(key){}

	bool operator()(const Elem& right)
	{
		return m_search == right.key;
	}
};


int testPtrFun1(const char* p)
{
	return atoi(p) + 10;
}

int testPtrFun2(const char* p, int q)
{
	return atoi(p) + q;
}


void example_algorithm()
{
	vecElem elems;
	elems.reserve(100);
	for (int i = 0; i < 100; i++)
	{
		Elem temp;
		temp.key = i % 10;
		elems.push_back(temp);
	}

	// use operator == (const Elem& right)
	Elem temp;
	temp.key = 3;
	elems.erase(remove(elems.begin(), elems.end(), temp), elems.end());

	// 1. use operator == (int key)
	// 2. use Elem(int) and operator == (const Elem& right)
	elems.erase(remove(elems.begin(), elems.end(), 4), elems.end());

	elems.erase(remove_if(elems.begin(), elems.end(), elem_equal), elems.end());

	elems.erase(remove_if(elems.begin(), elems.end(), ElemCmp(5)), elems.end());
	

	vecElem buffer1, buffer2;
	remove_copy(elems.begin(), elems.end(), 
		inserter(buffer1, buffer1.end()), 1);

	remove_copy_if(elems.begin(), elems.end(), 
		inserter(buffer2, buffer2.end()), ElemCmp(0));

	
	char* foo[] = {"10","20","30","40","50"};
	int bar[5];
	transform (foo, foo+5, bar, ptr_fun(testPtrFun1));
	transform (foo, foo+5, bar, bind2nd(ptr_fun(testPtrFun2), 100));

	//not1();
	//not2();

	// ptr_fun : Returns a function object that encapsulates function f.

	//find();
	//binary_search();
	//for_each();
	//adjacent_find();
	//count();
	//count_if();
	//search();
	//search_n();
	//find_end();
	//find_first_of();
}

