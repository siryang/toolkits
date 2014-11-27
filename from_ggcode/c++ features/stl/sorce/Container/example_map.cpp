#include "stdafx.h"
#include "../example_container.h"



void mapErase()
{
	typedef map<int, int>  IntMapping;
	IntMapping mapContiner;
	for (int i = 0; i < 100; i++)
	{
		mapContiner.insert(make_pair(i % 7, i % 7));
	}

	for (IntMapping::iterator iter = mapContiner.begin()
		; iter != mapContiner.end();)
	{
		if (iter->first == 0)
		{
			iter = mapContiner.erase(iter);
			//mapContiner.erase(iter++);
			// map erase 使用上述两种方法结果一样
		}
		else
		{
			iter++;
		}
	}
}

void map_test()
{
	mapErase();
	exit(0);
}
