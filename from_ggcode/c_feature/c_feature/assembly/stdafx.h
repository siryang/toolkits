// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
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

// TODO: 在此处引用程序需要的其他头文件
