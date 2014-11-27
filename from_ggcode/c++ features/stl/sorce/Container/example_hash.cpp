#include "stdafx.h"
#include "../example_container.h"
#include <hash_set> // hash_set hash_multiset
using namespace stdext;

struct ElemHash
{
	enum
	{	// parameters for hash table
		bucket_size = 4,	// 0 < bucket_size
		min_buckets = 8
	};	// min_buckets = 2 ^^ N, 0 < N

	// make hash key;
	size_t operator() (const Elem& elem) const
	{
		return (size_t)elem.key;
	}

	// overload () for compare.
	bool operator()(const Elem& left, const Elem& right) const
	{
		return left.key == right.key;
	}
};


void hash_set_test()
{
	hash_set<Elem, ElemHash> hashSet;
	//hash_set<Elem, ElemHash, ElemEqual> elemSet;
	hashSet.insert(3);
	hashSet.insert(9);
	hashSet.insert(5);
	hashSet.find(1900);
	hashSet.lower_bound(4);
	
	
	hash_set<Elem> p;
	p.load_factor();
	
	
	//hashSet.comp
	//hash_multiset<int> hashMultiSet;
	//hash_set<Elem> elems;
}


void hash_map_test()
{

	hash_map<int, Elem> hashmap1;
	hash_map<int, Elem, ElemHash> hashmap2;


	//hash_map<int, Elem> hashMap;
	//hash_multimap<int, Elem> hashMultimap;
	//hash_map<int, Elem> elems;
}





