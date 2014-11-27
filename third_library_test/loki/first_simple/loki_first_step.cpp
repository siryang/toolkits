#include <stdlib.h>
#include <iostream>

#include <loki/Singleton.h>
#include "loki_first_step.h"

using namespace std;

class ValueSetClass
{
private:
	int m_value;
public:
	void showValue()
	{
		cout << "Value:" << m_value << endl;
	}

	void setValue(int value)
	{
		m_value = value;
	}
};

// CreateUsingNew/CreateUsingMalloc
// LongevityLifetime::DieAsSmallObjectChild/DieFirst/DieLast
typedef  Loki::SingletonHolder<ValueSetClass, CreateUsingNew> SingletonValueSet;
typedef Loki::

int main()
{
	SingletonValueSet::Instance().setValue(100);
	SingletonValueSet::Instance().showValue();
	system("pause");
	return 0;
}