#include "stdafx.h"
#include "../example_container.h"




// ������ɾ��Ԫ�ؾ���
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
	// remove �㷨��û������ɾ��Ԫ�ء����ǽ���Ҫ��Ԫ�ض��ƶ���ǰ�ߣ�ͬʱ���������һ��Ԫ�ص���һ��λ�á�
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
		<< "empty: " << (elems.empty()? "true": "false") << endl	//ʹ��empty����size, ���ڹ��������������Ч��
		<< "max size: " <<  elems.max_size() << endl				// fixit: �����ܹ����ص����������
		<< "capacity: " << elems.capacity() << endl;				// ����reallocʱ���������size

	elems.reserve(1000);											// ���û��������ȡ�
	cout << "reserve 1000, " << "capacity: " << elems.capacity() << endl;				// ��������С

	elems.resize(100);		// ���ó��ȡ�������;��1.Ԥ���������С�� 2.ɾ���󼸸�Ԫ��
	cout << "resize 100," << "size:" << elems.size() << endl; 

	elems.clear();			// clear��swap(T())������clear���ͷ��ڴ棬��������������ʱ�ͷš�
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

// �̳�vector,д�Լ���vectorģ��
//template<class _Ty,
//class _Ax>
//class SelfVector
//	:public vector
//{
//};