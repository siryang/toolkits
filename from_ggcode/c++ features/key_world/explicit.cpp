

// explicit  声明的只有一个参数的构造函数，参数不能使用隐式转换。

// spec1_explicit.cpp
// compile with: /EHsc
#include <stdio.h>

class C 
{
public:
	int i;
	explicit C(const C&)   // an explicit copy constructor
	{
		printf_s("\nin the copy constructor");
	}
	explicit C(int i )   // an explicit constructor
	{
		printf_s("\nin the constructor");
	}

	C()
	{
		i = 0;
	}
};

class C2
{
public:
	int i;
	explicit C2(int i )   // an explicit constructor
	{
	} 
};

C f(C c)
{   // C2558
	c.i = 2;
	return c;   // first call to copy constructor
}

void f2(C2)
{
}

void g(int i)
{
	f2(i);   // C2558
	// try the following line instead
	// f2(C2(i));
}

int main()
{
	C c, d;
	d = f(c);   // c is copied
}