// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>


template<class T>
class TempTest
{
public:
	const static TempTest<T>* instance()
	{
		if (m == NULL)
			m = new TempTest<T>;
		return m;
	}

	void show() const
	{
		;
	}
private:
	int p;
	static TempTest<T>* m;
};


template<class T>
TempTest<T>* TempTest<T>::m  = NULL;

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
