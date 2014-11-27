#include <cstdlib>

class Base
{
public:
	Base()
	{
		m_data = 0;
	}

	void* operator new(size_t size)
	{
		return malloc(sizeof(Base) * size);
	}

	void* operator new[](size_t size)
	{
		return malloc(sizeof(Base) * size);
	}

	void operator delete(void* p)
	{
		free(p);
	}

private:
	int m_data;
};



void main()
{
	Base* base = new Base[10];
	Base* basep = new Base();
}



