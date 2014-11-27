#pragma once
//写一个map<ObjectId, ObjectId>的替代品
// 这种方法一点都不好，不如使用hash_map.
// 周末自己写一个HashMap吧，可以参考冯的。

#include <vector>
#include <algorithm>
using namespace std;

template<typename KeyType, typename ValueType>
class Key2Key;

template<typename KeyType, typename ValueType>
class Key2Key
{
public:
	struct InnerKey
	{
		Key2Key key;
		size_t index;

		bool operator > (const InnerKey& right)
		{
			return key > right.key;
		}

		InnerKey(){}

		InnerKey(size_t _i, InnerKey _key): index(_i), key(_key);
		{}

	};

	Key2Key()
	{
		m_sorted = false;
	}

	void reverse(size_t size)
	{
		m_keys.reverse(size);
		m_values.reverse(size);
	}

	void resize(size_t size)
	{
		m_keys.resize(size);
		m_values.resize(size);
	}

	ValueType* insert(KeyType key, ValueType value)
	{
		m_sorted = false;
		m_keys.push_back(key);
		m_values.push_back(value);
		return &m_values.back();
	}

	ValueType* find(KeyType key)
	{
		if (!m_sorted)
			sort();

		vector<KeyType>::iterator iter = binary_search(m_keys.begin(), m_keys.end(), key);
		if (iter == m_keys.end())
			return NULL;
		
		return &m_values[iter - m_keys.begin()];
	}

	ValueType& operator [](KeyType key)
	{
		ValueType* p = find(key);
		if (p)
			return *p;
		else
		{
			return *insert(key, ValueType());
		}
	}

	size_t size()
	{
		return m_keys.size();
	}

	typedef size_t iterator;

	iterator iterator_begin()
	{
		return 0;
	}

	iterator iterator_end()
	{
		return m_keys.size();
	}

	iterator iterator_plus(iterator& iter)
	{
		return ++iter;
	}

	ValueType& iterator_data(iterator iter)
	{
		return m_values[iter];
	}

private:

	void sort()
	{
		vector<size_t> index;
		
		m_sorted = true;
				
	}

	bool m_sorted;
	vector<InnerKey> m_keys;
	vector<ValueType> m_values;
};
