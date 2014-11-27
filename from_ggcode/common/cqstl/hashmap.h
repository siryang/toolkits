#pragma once

extern "C"
{
#include "cq_math.h"
}

#include "vector.h"

#undef INCREASE
#undef DECREASE

#define INCREASE(i) i++; if (i == m_tableSize) i = 0
#define DECREASE(i) if (i == 0) i = m_tableSize - 1; else i--
#define IN_USE(i) ((m_inUse)[(i) / 8] & (1 << ((i) % 8)))
#define SET_IN_USE(i) m_inUse[(i) / 8] |= 1 << ((i) % 8)
#define SET_NOT_USE(i) m_inUse[(i) / 8] &= ~(1 << ((i) % 8))

#define NOT_FOUND (size_t)(-1)

namespace cqstd
{
	template<typename T>
	class DefaultHasher
	{
	public:
		static size_t hash(const T& t) {
			return (size_t)t;
		}
		static bool equal(const T& l, const T& r) {
			return l == r;
		}
	};

	/**
		A hash map container.

		Unlike STL, which is bloated, slow and hard to use, this implementation of
		hash map is designed with both efficiency and ease of use in mind. But to achieve 
		that goal. There are one limitation. It can only use POD as key and value.

		There are 2 setters. set() and insert().
		There are 2 getters. get() and find().
		There are 2 delete functions. remove() and erase().
		These functions behave just like STL's hash_map.

		The iterator is dramatically different from STL.
		@code
		Hashmap<size_t, int>::iterator iter(&map);
		while(iter.next())
		{
			if (iter.key == 1 or iter.value == 1)
			{
				map.erase(&iter);	// you can delete some items during iteration.
			}
		}
		@endcode

		There are two ways to use a customized type as the KeyType.
		1. Add a converter function.
			struct Point {
				int x, y;
				operator size_t() const {
					return x + y;
				}
			};
			Hashmap<Point, size_t> map;
		2. Define a hasher class. This is very useful if method one is not possible or convenient.
			class StringHasher
			{
			public:
				static size_t hash(char* s) {
					size_t rtn = 0;
					while(*s)
						rtn += *s++;
					return rtn;
				}
				static bool equal(char* l, char* r)	{
					return cq_strcmp(l, r) == 0;
				}
			};
			Hashmap<char*, size_t> map;
	*/
	template<typename KeyType, typename ValueType, typename Hasher = DefaultHasher<KeyType> >
	class Hashmap
	{
	private:
		enum Error{
			Error_missing = -3,  /* No such element */
			Error_full = -2, 	/* Hashmap is full */
			Error_omem = -1, 	/* Out of Memory */
			Error_ok =  0 	/* OK */
		};

		struct element{
			KeyType key;
			ValueType value;
		};

	public:
		class iterator
		{
			friend class Hashmap;
		private:
			size_t m_tableSize;
			element* m_data;
			char* m_inUse;
			size_t m_i, m_end;

			iterator() {}

		public:
			KeyType key;
			ValueType value;

			iterator(Hashmap* map)
			{
				reset(map);
			}

			void reset(Hashmap* map)
			{
				size_t i;

				m_tableSize = map->m_tableSize;
				m_data = map->m_data;
				m_inUse = map->m_inUse;

				if (map->m_size == 0)
				{
					m_i = (size_t)-1;
					m_end = 0;
					return;
				}

				// find the first object with a hole before it.
				for(i = 0; i < m_tableSize; i++)
				{
					// find first hole
					if (!IN_USE(i))
					{
						// pass the hole
						for(; i < m_tableSize; i++)
						{
							if (IN_USE(i))
								break;
						}
						break;
					}
				}
				i = i % m_tableSize;
				m_end = m_i = i;
				DECREASE(m_i);
				DECREASE(m_end);
				key = m_data[i].key;
				value = m_data[i].value;
			}

			bool next() 
			{
				CQ_ASSERT(m_data != NULL);	// some iterators are for deletion only and cannot be used to iterate all members.

				INCREASE(m_i);

				if (m_i == m_end)
					return false;

				while(m_i != m_end && !IN_USE(m_i))
				{
					INCREASE(m_i);
				};

				if (m_i != m_end)
				{
					key = m_data[m_i].key;
					value = m_data[m_i].value;
					return true;
				}
				else
				{
					return false;
				}
			}
		};

	public:
		Hashmap() {
			_reserve(16);
		}

		Hashmap(size_t bucketSize) {
			_reserve(bucketSize);
		}

		~Hashmap() {
			free(m_data);
			free(m_inUse);
		}

		void clear() {
			memset(m_inUse, 0, (m_tableSize + 7) / 8);
			m_size = 0;
		}

		bool empty() {
			return m_size == 0;
		}

		size_t size() {
			return m_size;
		}

		size_t bucketSize() {
			return m_tableSize;
		}

		void set(const KeyType& key, const ValueType& value)
		{
			size_t index = 0;
			Error err = _findInsertPosition(key, &index);

			if (err != Error_ok)
				return;

			/* Set the data */
			if (!IN_USE(index))
				m_size++;

			m_data[index].value = value;
			m_data[index].key = key;
			SET_IN_USE(index);
		}

		bool insert(const KeyType& key, const ValueType& value, iterator* iter = NULL)
		{
			size_t index = 0;
			Error err = _findInsertPosition(key, &index);

			if (err != Error_ok || IN_USE(index))
			{
				if (iter != NULL)
				{
					iter->m_data = NULL;
					iter->m_i = index;
					iter->key = key;
					iter->value = m_data[index].value;
				}
				return false;
			}

			m_size++;
			m_data[index].value = value;
			m_data[index].key = key;
			SET_IN_USE(index);

			return true;
		}

		bool get(const KeyType& key, ValueType* value) const
		{
			size_t curr = _find(key);
			if (curr != NOT_FOUND)
			{
				*value = m_data[curr].value;
				return true;
			}
			else
			{
				return false;
			}
		}

		bool find(const KeyType& key) const
		{
			size_t curr = _find(key);
			return curr != NOT_FOUND;
		}

		bool find(const KeyType& key, iterator* iter) const
		{
			size_t curr = _find(key);
			if (curr != NOT_FOUND)
			{
				iter->m_data = NULL;
				iter->m_i = curr;
				iter->key = key;
				iter->value = m_data[curr].value;
				return true;
			}
			else
			{
				return false;
			}
		}

		bool remove(const KeyType& key)
		{
			size_t curr = _find(key);
			if (curr != NOT_FOUND)
			{
				iterator iter;
				iter.m_i = curr;
				erase(&iter);
				return true;
			}
			else
			{
				return false;
			}
		}

		void erase(iterator* iter)
		{
			size_t curr;

			curr = iter->m_i;

			/* Blank out the fields */
			SET_NOT_USE(curr);

			/* Reduce the size */
			m_size--;

			/* Calculate the number of elements that need rehash*/
			size_t num = 0;
			{
				size_t p = curr;
				INCREASE(p);
				while(IN_USE(p))
				{
					num++;
					INCREASE(p);
				}
			}

			element* rehashArr = (element*)malloc(sizeof(element) * num);

			/* Store all succeeding elements into a temporary array.
				Because maybe they were pushed into the current place. 
				With an item removed, they will need to be rehashed. */
			element* p = rehashArr;
			INCREASE(curr);
			while(IN_USE(curr))
			{
				*p++ = m_data[curr];
				SET_NOT_USE(curr);
				INCREASE(curr);
				m_size--;
			}

			/*
				Rehash these stored elements
			*/
			p = rehashArr;
			element* pEnd = rehashArr + num;
			for(; p < pEnd; p++)
			{
				set(p->key, p->value);
			}

			free(rehashArr);
		}

		void swap(Hashmap<KeyType, ValueType, Hasher>* r)
		{
			cq_swap(element*, m_data, r->m_data);
			cq_swap(char*, m_inUse, r->m_inUse);
			cq_swap(size_t, m_tableSize, r->m_tableSize);
			cq_swap(size_t, m_size, r->m_size);
		}

	private:
		size_t _find(const KeyType& key) const
		{
			size_t curr;
			size_t i;

			/* Find data location */
			curr = Math_hashSizeT(Hasher::hash(key)) % m_tableSize;

			/* Linear probing, if necessary */
			for(i = 0; i< m_size; i++) {
				bool isUse = IN_USE(curr) != 0;
				if(isUse && Hasher::equal(m_data[curr].key, key)) {
					return curr;
				}
				else if (!isUse)
					break;

				curr = (curr + 1) % m_tableSize;
			}

			/* Not found */
			return NOT_FOUND;
		}

		Error _findInsertPosition(const KeyType& key, size_t* pos)
		{
			Error err;

			/* Find a place to put our value */
			err = _hash(key, pos);
			while(err == Error_full){
				if (_rehash() == Error_omem) {
					CQ_ASSERT(!"Out of memory");
					return Error_omem;
				}
				err = _hash(key, pos);
			}

			return err;
		}

		Error _hash(const KeyType& key, size_t* index)
		{
			size_t curr;
			size_t i;

			/* If 3/4 full, return immediately */
			if (m_size + 1 > (m_tableSize >> 1) + (m_tableSize >> 2))
				return Error_full;

			/* Find the best index */
			curr = Math_hashSizeT(Hasher::hash(key)) % m_tableSize;

			/* Linear probling */
			for(i = 0; i< m_tableSize; i++){
				if(!IN_USE(curr) || Hasher::equal(m_data[curr].key, key))
				{
					*index = curr;
					return Error_ok;
				}

				curr = (curr + 1) % m_tableSize;
			}

			return Error_full;
		}

		Error _rehash()
		{
			Hashmap<KeyType, ValueType, Hasher> tmp(m_tableSize * 2);

			/* Rehash the elements */
			for(size_t i = 0; i < m_tableSize; i++) 
			{
				if (IN_USE(i))
				{
					tmp.set(m_data[i].key, m_data[i].value);
				}
			}

			swap(&tmp);

			return Error_ok;
		}

		void _reserve(size_t bucketSize) 
		{
			m_data = (element*)malloc(bucketSize * sizeof(element));
			m_inUse = (char*)malloc(((bucketSize + 7) / 8) * sizeof(char));
			memset(m_inUse, 0,((bucketSize + 7) / 8) * sizeof(char));

			m_tableSize = bucketSize;
			m_size = 0;
		}

	private:
		size_t m_tableSize;
		size_t m_size;
		element* m_data;
		char* m_inUse;
	};
}

#undef INCREASE
#undef DECREASE
#undef IN_USE
#undef SET_IN_USE
#undef SET_NOT_USE
#undef NOT_FOUND
