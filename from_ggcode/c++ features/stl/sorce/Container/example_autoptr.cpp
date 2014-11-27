#include "stdafx.h"
#include "../example_container.h"
typedef auto_ptr<Elem> ElemPtr;

void autoptr_test();
void autoptr_example1();
ElemPtr autoptr_example2();


void autoptr_test()
{
	Elem* buffer = new Elem;
	auto_ptr<Elem> pElem(buffer);
	
	// Test code.
	pElem->key *= 2;
	pElem->value = &pElem->key;
}


inline void f(auto_ptr<Elem> ap)
{
	*ap;
}

void autoptr_example1()
{
	// Error 1
	Elem* temp = new Elem;
	ElemPtr elemPtr(temp);
	ElemPtr elemPtr2(temp);
	// in function ~auto_ptr(); use delete but not set pointer to null.

	// Error 2
	temp = new Elem[100];
	ElemPtr elemPtrArray(temp);
	// use delete not delete[]

	// Error 3
	auto_ptr<Elem> pElem(temp);
	auto_ptr<Elem> pElem2 = pElem; // "pElem" invalid��
	if (pElem.get() == NULL){
		cout << "old auto_ptr is invalid " << endl;
	}
	// see also
	f(pElem); 
	// ��������ʱ�������һ���µ�auto_ptr������ֵ����ʹ�ɵ�auto_ptrʧЧ, ʹ��const auto_ptr<>&�ǲ����ѡ�񡣻���ֱ����get

}

ElemPtr autoptr_example2()
{
	// Use like this.
	ElemPtr pElem(new Elem);
	return pElem;
}

