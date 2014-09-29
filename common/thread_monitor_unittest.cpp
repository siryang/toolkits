#include <gtest/gtest.h>
#include <iostream>
#include "thread_monitor.h"

using namespace std;

#define SHOW_HELLO_EVENT 1
#define SHOW_WELCOME_EVENT 2

void show(const char* str)
{
	cout << str << endl;
}

TEST(MonitorThread, test)
{
	MonitorThread monitor;

	//monitor.registe()
}