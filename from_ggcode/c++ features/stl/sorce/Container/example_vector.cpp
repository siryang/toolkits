#include "stdafx.h"
#include "../example_container.h"




// 迭代器删除元素举例
void vector_earse()
{
	vector<int> p;
	p.reserve(100);
	for (int i = 0; i < 100; i++)
	{
		p.push_back(i % 7);
	}

	for (vector<int>::iterator iter = p.begin(); iter != p.end();)
	{
		if (*iter == 0)
		{
			//p.erase(iter++); // error
			iter = p.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	p.erase(remove(p.begin(), p.end(), 3), p.end());
	// remove 算法并没有真正删除元素。而是将需要的元素都移动到前边，同时，返回最后一个元素的下一个位置。
	// original 1 2 3 4 5 6 3    
	// remove 1 2 4 5 6 6 3		return index of second 6.
	// earse  1 2 4 5 6			real erase.


	//bind2nd();
	//bind1st();
}

void vector_member()
{
	vecElem elems;
	vector_earse();
	//vecElem::size_type;
	//vecElem::allocator_type;
	//vecElem::difference_type;
	//vecElem::value_type;
	//
	//vecElem::reference;
	//vecElem::pointer;
	//vecElem::iterator;
	//vecElem::reverse_iterator;
	//
	//vecElem::const_iterator;
	//vecElem::const_pointer;
	//vecElem::const_iterator;
	//vecElem::const_reverse_iterator;

	cout << "size: " << elems.size() << endl
		<< "empty: " << (elems.empty()? "true": "false") << endl	//使用empty代替size, 对于关联型容器提高了效率
		<< "max size: " <<  elems.max_size() << endl				// fixit: 容器能够承载的最大数据量
		<< "capacity: " << elems.capacity() << endl;				// 不需realloc时，可用最大size

	elems.reserve(1000);											// 设置缓冲区长度。
	cout << "reserve 1000, " << "capacity: " << elems.capacity() << endl;				// 缓冲区大小

	elems.resize(100);		// 设置长度。两种用途，1.预申请数组大小， 2.删除后几个元素
	cout << "resize 100," << "size:" << elems.size() << endl; 

	elems.clear();			// clear和swap(T())的区别，clear不释放内存，到析构函数调用时释放。
	cout << "clear, size:" << elems.size() << " capacity: " << elems.capacity() << endl;

	elems.resize(20);	
	elems.at(10);

	elems.front();
	elems.back();

	elems.begin();
	elems.end();

	elems.rbegin();			// reverse
	elems.rend();

	Elem buffer[30];
	vecElem vecBuffer;
	vecBuffer.assign(&buffer[0], &buffer[29]);
	elems.assign(10, buffer[0]);

	elems.insert(elems.begin(), 10, buffer[1]);
	elems.insert(elems.begin() + 1, buffer[2]);
	elems.insert(elems.end(), vecBuffer.begin(), vecBuffer.end());

	elems.erase(elems.begin() + 4);
	elems.push_back(buffer[3]);
	elems.pop_back();
	elems.swap(vecBuffer);

	//alloc 
	vecElem::allocator_type alloc = elems.get_allocator();		//fixit
	exit(0);
}

// 继承vector,写自己的vector模板
//template<class _Ty,
//class _Ax>
//class SelfVector
//	:public vector
//{
//};