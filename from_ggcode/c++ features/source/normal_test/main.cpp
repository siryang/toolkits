#include "common_header.h"
#include <Windows.h>
#include <vector>

using namespace std;

// ==begin
// Test performance of struct destruct function.
struct A
{
	int* m;

	~A()
	{
		if (m != NULL)
			delete m;
	}
};

struct B
{
	int* m;
};

void free(B& b)
{
	if (b.m != NULL)
	{
		delete b.m;
	}
}


void testA(size_t arraySize)
{
	A* array  = new A[arraySize];
	for (size_t i = 0; i < arraySize; i++)
	{
		array[i].m = NULL;
	}
	delete[] array;
}

void testB(size_t arraySize)
{
	B* array  = new B[arraySize];
	for (size_t i = 0; i < arraySize; i++)
	{
		array[i].m = NULL;
	}

	for (size_t i = 0; i < arraySize; i++)
	{
		free(array[i]);
	}

	delete[] array;
}

void testB2(size_t arraySize)
{
	B* array  = new B[arraySize];
	for (size_t i = 0; i < arraySize; i++)
	{
		array[i].m = NULL;
	}

	delete[] array;
}

void perfermanceOfTestAB()
{
	double start, end;

	start = GetTickCount();
	testA(10 * 1024 * 1024);
	end = GetTickCount();
	cout << end - start << endl;

	start = GetTickCount();
	testB(10 * 1024 * 1024);
	end = GetTickCount();
	cout << end - start << endl;

	start = GetTickCount();
	testB2(10 * 1024 * 1024);
	end = GetTickCount();
	cout << end - start << endl;
}
// === end test struct destruct performance.

class N
{
	class M
	{
		friend N;
	private:
		int num;
	};

	void setNum(int num)
	{
		m.num = num;
	}

	void showNum()
	{
		cout << m.num << endl;
	}
public:
	static void test()
	{
		N n;
		n.setNum(1);
		n.showNum();
	}
private:
	M m;
};

namespace glmap
{
	class Base
	{
	protected:
		int m;
	};
}

namespace glmap3
{
	using namespace glmap;
	class Base: public glmap::Base
	{
	public:
		void setValue(int v)
		{
			m = v;
		}

		void show()
		{
			cout << "Value" << m << endl;
		}
	};
}

class ConstructTest
{
public:
	ConstructTest(
		bool a_bIsUtf8    = false,
		bool a_bMultiKey  = false,
		bool a_bMultiLine = false)
	{
		cout << a_bIsUtf8 << a_bMultiKey << a_bMultiLine << endl;
		// will show 100
	}

	void show()
	{
		cout << "this is show!" << endl;
	}
};


// == begin test call function has same name in base class
class Base
{
protected:
	void show(){
		cout << "i am base" << endl;
	}
};

class Child: public Base
{
public:
	Child()
	{
		static int i = 0;
		cout << "call child constructor: " << ++i << endl;
	}

	Child(const Child& right)
	{
		static int i = 0;
		cout << "call copy function" << ++i << endl;
	}

	void show(){
		Base::show();
		cout << "i am child" << endl;
	}

	static void test()
	{
		Child test;
		test.show();
		exit(0);
	}
};

Child returnChild()
{
	Child c;
	c;
	return c;
}

int main()
{
	vector<Child> p(10);
	p.resize(20);
	Child c;
	p.push_back(c);
	p.push_back(c);
	p.push_back(returnChild());


	Child::test();

	ConstructTest test(L"123");
	test.show();

	glmap::Base* base = new glmap3::Base;
	
	error_example1_main();
	//error_example_2_main();
	system("pause");
}


