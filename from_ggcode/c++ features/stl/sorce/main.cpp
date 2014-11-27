// Example.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "example_container.h"
#include "example_iterator.h"
#include "example_algorithm.h"

void vecAssignTest()
{
	vector<string> m;
	m.push_back("");

	vector<int> p;
	p.push_back(1);
	p.push_back(2);
	p.push_back(3);

	vector<int> q;
	q.assign(p.rbegin(), p.rend());
	q.push_back(4);
	q.push_back(5);
	q.push_back(6);
}

// auto_ptr
class AutoClass
{
public:
	~AutoClass()
	{
		cout << "This will auto call by auto_ptr" << endl;
	}

	static AutoClass* instance()
	{
		if (autoPointer.get() == 0)
		{
			autoPointer.reset(new AutoClass());
		}
		return autoPointer.get();
	}

private:
	AutoClass()
	{
		cout << "NEW AutoClass" << endl;
	}

	static auto_ptr<AutoClass> autoPointer;
};

auto_ptr<AutoClass> AutoClass::autoPointer;

void auto_ptr_test()
{
	AutoClass::instance();
}


int _tmain(int argc, _TCHAR* argv[])
{
	//vecAssignTest();
	map_test();
	vector_member();
	set_test();
	hash_set_test();
	iterator_test2();
	example_algorithm();
	example_iterator();
	list_test();
	autoptr_test();
	return 0;
}

