// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <list>

using namespace std;

struct Elem
{
	int key;
	void* value;

	Elem()
	{
		static int keyPool = 0;
		key = keyPool++;
		value = NULL;
	}

	Elem(int _Key)
	{
		key = _Key ;
		value = NULL;
	}

	bool operator == (const Elem& right)
	{
		return key == right.key;
	}

	bool operator == (int rightKey)
	{
		return key == rightKey;
	}
	
	size_t operator()(const Elem &t) const
	{
		return (size_t)t.key;
	}
	//bool operator != (int rightKey)
	//{
	//	return key != rightKey;
	//}
};

/*
	STLÁù´ó×é¼þ
	1.Container
	2.Algorithm
	3.Iterator
	4.Functors
	5.Adapters
	6.Allocators





*/


