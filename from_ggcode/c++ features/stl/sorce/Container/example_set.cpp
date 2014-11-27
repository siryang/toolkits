#include "stdafx.h"
#include "../example_container.h"

struct SetTestUnit
{
	int a;
	int b;
};
//inline bool operator < (const SetTestUnit& left, const SetTestUnit& right)
//{
//	return left.a < right.a && left.b < right.b;
//}

struct SetTestUnitLess
{
	inline bool operator()(const SetTestUnit& left, const SetTestUnit& right)
	{
		return left.a < right.a && left.b < right.b;
	}
};


void set_test_user_struct()
{
	set<SetTestUnit, SetTestUnitLess> units;
	for (int i = 0; i < 100; i++)
	{
		SetTestUnit unit = {i % 7, i % 7 + 1};
		units.insert(unit);
	}
}

void set_test()
{
	set_test_user_struct();

	set<int> p;
	for (int i = 0; i < 100; i++)
	{
		p.insert(i % 7);
	}
	
	p.empty();
	p.size();
	p.count(7);
	p.upper_bound(3);
	p.find(3);
	p.lower_bound(3);
	p.clear();

	p.key_comp();//?
	p.value_comp(); //?
	p.get_allocator();
	p.max_size();
	
	p.begin();
	p.end();
	p.rbegin();
	p.rend();

	p.insert(9);
	p.erase(8);
}
void multiset_test()
{
	set<int> p;
	pair<set<int>::iterator, set<int>::iterator> four = p.equal_range(4);

}

void bitset_test()
{

}
