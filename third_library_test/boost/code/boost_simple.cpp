#include "boost_simple.h"
#include <iostream>
#include <boost/thread.hpp>

using namespace std;
using namespace boost;

int test_value = 0;

void simple_thread()
{
	test_value += 100;
	cout << test_value << endl;
}

void boostThreadSimple()
{
	thread myThread(simple_thread);
	thread myThread1(simple_thread);
	thread myThread2(simple_thread);
	thread myThread3(simple_thread);

	myThread.join();
	myThread1.join();
	myThread2.join();
	myThread3.join();

	system("pause");
}

#ifdef BOOST_THREAD_SIMPLE
int main()
{
	boostThreadSimple();
	return 0;
}
#endif
