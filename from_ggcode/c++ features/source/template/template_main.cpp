#include <cstdlib>
#include <iostream>

#include <map>
#include <hash_map>
using namespace std;
using namespace stdext;

template<bool isPointer, typename value_type>
class Unit
{
public:
	Unit(value_type b):m_data(b){};
	
	~Unit()
	{
		if (isPointer)
		{
			std::cout << "pointer. " << std::endl;
		}
		else
		{
			std::cout << "not pointer." << std::endl;
		}
	}

	void show()
	{
		if (isPointer)
			std::cout << *m_data << std::endl;
		else
			std::cout << m_data << std::endl;
	}

private:
	value_type m_data;
};

void testUnit()
{
	int* p = new int;
	int b = 3;
	*p = 4;

	Unit<true, int*> unitTrue(p);
	//Unit<false, int> unitFalse(b);

	//unitFalse.show();
	unitTrue.show();
	delete p;
}

void testMapMemory(size_t num)
{
	__debugbreak();
	map<size_t, size_t> mapKey;

	for (size_t i = 0; i < num; i++)
	{
		mapKey.insert(pair<size_t, size_t>(i, i));
	}
	__debugbreak();
	mapKey.clear();
}

void testMallocMemoryCost(size_t num)
{
	__debugbreak();
	for (size_t i = 0; i < num; i++)
	{
		malloc(sizeof(char));
	}
	__debugbreak();
}

void testHashMapMemoryCost(size_t num)
{
	__debugbreak();
	hash_map<int, int> hashm;
	for (size_t i = 0; i < num; i++)
	{
		hashm.insert(pair<int, int>(i, i));
	}
	__debugbreak();
}


class TestCPP
{
public:
	void release()
	{
		m_int[1] = 124;
		delete this;
	};

	~TestCPP()
	{
		cout << m_int[1] << endl;
	}
private:
	int m_int[20];
};

int main()
{
	TestCPP m;
	m.release();
	delete &m;
	system("pause");
	return 0;
}

