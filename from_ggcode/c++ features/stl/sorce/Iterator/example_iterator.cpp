#include "stdafx.h"
#include "../example_iterator.h"


void example_iterator()
{
	VecInt vecTest;
	VecInt::iterator iter;
	VecInt::reverse_iterator riter;
	//VecInt::reverse_bidirectional_iterator rBiIter;

	VecInt::const_iterator citer;
	VecInt::const_reverse_iterator criter;


	vecTest.reserve(100);
	for (int i = 0; i < 100; i++)
	{
		vecTest.push_back(i % 10);
	}
	
	VecInt::reverse_iterator riterEnd = vecTest.rend();

	for (riter = vecTest.rbegin();
		riter != riterEnd; )
	{
		if (*riter % 2 == 0)
		{
			iter = (riter++).base();

			cout << *riter << endl;	// [98]
			cout << *iter << endl;		// [99]
			//==============================
			//											 rbegin  
			//  rend  O O O O  |  O     O  O 
			//-------------------------------------------------	
			//           O O O O     O  |  O  O end
			//       begin 
			//==============================

			vecTest.erase(iter--);
			cout << *riter << endl;
		}
		else
			riter++;
	}

	iter = riter.base();

	//删除元素导致迭代器失效的问题。
	// erase-remove
}

void iterator_test2()
{
	//insert_iterator;
	const int N = 6;

	int A1[N] = {1, 3, 5, 7, 9, 11};
	int A2[N] = {1, 5, 3, 4, 5, 6};
	set<int> result;
	
	result.insert(4);

	insert_iterator<set<int>> insertIterator = inserter(result, result.begin());
	merge(A1, A1 + N, A2, A2 + N, insertIterator);

	copy(result.begin(), result.end(), ostream_iterator<int>(cout, " "));

	VecInt temp(10);
	for ( int i = 0; i < 10; i++)
	{
		temp.push_back(i * 9);
	}

	back_insert_iterator<VecInt> backInsertIter = back_inserter(temp);
	copy(result.begin(), result.end(), backInsertIter);

	deque<int> p;
	front_insert_iterator<deque<int>> frontInsert = front_inserter(p);
	copy(result.begin(), result.end(), frontInsert);
	cout << endl;
	/*
	checked_iterator;*/
}

void iterator_test3()
{
	//istream_iterator;
	//istreambuf_iterator;
	//ostream_iterator;
}

