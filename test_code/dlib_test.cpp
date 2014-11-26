#include "gtest/gtest.h"
#include <dlib/queue.h>
#include <iostream>

using namespace std;

typedef dlib::queue<int>::sort_1a IntQueue;

TEST(DLib, queue)
{
	IntQueue myQueue;
	for (int i = 0; i < 10; i++)
	{
		int a = i * (i % 3);
		cout << "input" << a << endl;
		myQueue.enqueue(a);
	}

	while(myQueue.move_next())
	{
		cout << myQueue.element() << endl;
	}
}