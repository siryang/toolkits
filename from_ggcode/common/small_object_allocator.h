#pragma once

template <typename T>
class SmallObjectAllocator
{
private:
	static const size_t m_poolSlotSize = 1024 * 4;	// 4k

	size_t m_objNum;
	vector<T*> m_pool;	// one slot of the pool contains m_poolSlotSize objects.
	vector<T*> m_freeList;
	
	// in free list, use struct {len, pointer} store, or use 
	// len = 1: list, len = 2: list..., len > 50: list
public:
	SmallObjectAllocator() : m_objNum(0)
	{
	}

	virtual ~SmallObjectAllocator()
	{
		freeAllObjects();
	}

	T* allocObj(size_t num = 1)
	{
		if (!m_freeList.empty() && num == 1)
		{
			T* rtn = m_freeList.back();
			m_freeList.pop_back();
			return rtn;
		}

		size_t slot = m_objNum / m_poolSlotSize;
		size_t index = m_objNum % m_poolSlotSize;

		// actualy num shoule < m_poolSlotSize / 3
		_ASSERT(num <= m_poolSlotSize);
		if ((m_objNum + num) >= m_pool.size() * m_poolSlotSize)
		{
			// push obj in slot not used to free list.
			if (!m_pool.empty())
			{
				m_objNum += m_poolSlotSize - index;
				while (index < m_poolSlotSize)
					m_freeList.push_back(&m_pool[slot][index++]);
			}

			m_pool.resize(m_pool.size() + 1);
			m_pool.back() = new T[m_poolSlotSize];
			slot = m_pool.size() - 1;
			index = 0;
		}

		m_objNum += num;
		return &m_pool[slot][index];
	}

	void freeObj(T* obj, size_t num = 1)
	{
		// here has a bug, when a pointer be free twice, it will store double time in freeList, error when alloc.
		// to avoid this, we can use set..but it use more memory
		for (size_t i = 0; i < num; i++)
		{
			obj[i].clear();
			m_freeList.push_back(obj + i);
		}
	}

	void freeAllObjects()
	{
		for each(T* slot in m_pool)
		{
			delete[] slot;
		}
		m_pool.clear();
		m_freeList.clear();
		m_objNum = 0;
	}

	bool isPointerInFreeList(T *obj)
	{
		return find(m_freeList.begin(), m_freeList.end(), obj) != m_freeList.end();
	}

	size_t getDataNumber()
	{
		return m_objNum;
	}
};

/**
	this is suit to alloc memory but never want to free in running.
*/
template <typename T>
class FixedObjectAllocator
	: public SmallObjectAllocator<T>
{
public:
	typedef FixedObjectAllocator<T> Allocator;
	static Allocator* instance()
	{
		if (!m_instance)
		{
			m_instance = new Allocator;
		}
		return m_instance;
	}

private:
	void freeObj(T* obj, size_t num = 1);

	FixedObjectAllocator()
	{}

	static Allocator* m_instance;
};
template<typename T>
__declspec(selectany) FixedObjectAllocator<T>* FixedObjectAllocator<T>::m_instance = NULL;

