#include <gtest/gtest.h>
#include <iostream>

using namespace std;

#define declare(type, var)\
private:\
	type m_##var;

#define declare_get(type, var)\
public:\
	type get_##var(){return m_##var;}

#define declare_set(type, var)\
public:\
	void set_##var(type var){m_##var = var;}

#define var_r(type, var)\
	declare(type, var);\
	declare_get(type, var);

#define var_rw(type, var)\
	declare(type, var);\
	declare_get(type, var);\
	declare_set(type, var);


class TestVar
{
	// declare var in ruby style.
	var_r(int, rVar);
	var_rw(int, rwVar);
public:
	TestVar()
	{
		m_rVar = 1;
		m_rwVar = 2;
	}
};

TEST(VarDeclare, basic)
{
	TestVar test;
	cout << test.get_rVar() << endl;
	cout << test.get_rwVar() << endl;

	test.set_rwVar(100);
	cout << test.get_rwVar() << endl;
}