#pragma once


/// for hash_set<wchar_t*>, define a comparision function
struct wchar_t_less
	: public binary_function<const wchar_t*, const wchar_t*, bool>
{	// functor for operator<
	bool operator()(const wchar_t* _Left, const wchar_t* _Right) const
	{	// apply operator< to operands
		return wcscmp(_Left, _Right) < 0;
	}
};

typedef stdext::hash_set<const wchar_t*, hash_compare< const wchar_t*, wchar_t_less > > StringPool;


/// for hash_set<ItemType*>, define a comparision function
struct BasicSegmentData_less
	: public binary_function<const BasicSegmData*, const BasicSegmData*, bool>
{	// functor for operator<
	bool operator()(const BasicSegmData* _Left, const BasicSegmData* _Right) const
	{	// apply operator< to operands
		const char* p = (const char*)_Left;
		const char* q = (const char*)_Right;
		for (size_t i = 0; i < sizeof(BasicSegmData); i++)
		{
			if (p[i] != q[i])
				return p[i] < q[i];
		}
		
		return false;
	}
};

template<class ItemType, class ItemType_less>
typedef stdext::hash_set<const ItemType*, hash_compare<const ItemType*, ItemType_less> > BasicSegmentDataPool;




typedef size_t FlyWeightKey;
class FlyWeightUnit
{
public:
	FlyWeightUnit();

	virtual FlyWeightKey toKey();

private:
	size_t m_referenceTime;
};

class FlyWeight
{
public:
	FlyWeightKey add(const FlyWeightUnit& data)
	{
		FlyWeightKey key = data->toKey();
		FlyWeightPool::iterator iterDataItem = m_pool.find(key);
		if (iterDataItem == m_pool.end())
		{
			m_pool.insert(pair<FlyWeightKey, FlyWeightUnit*>(key, ));
		}
		return key;
	}

private:
	typedef map<FlyWeightKey, FlyWeightUnit*> FlyWeightPool;
	 
	FlyWeightPool m_pool;
};
