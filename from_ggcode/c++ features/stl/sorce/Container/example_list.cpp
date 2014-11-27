#include "stdafx.h"
#include "../example_container.h"

typedef list<Elem> ElemList;

void list_test()
{
	Elem temp;
	ElemList elems;
	elems.push_front(temp);
	elems.pop_front();


	
	list<int> p;
	for (int i = 0; i < 100; i++)
	{
		p.push_back(i % 10);
	}

	p.remove(9);
	
	// at remove function use "iterator = list.earse(iterator)" to erase element.

	//elems.merge();
	//elems.merge();
}
