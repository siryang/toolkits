#pragma once
#include "basic_type.h"

class ThridLibSample
{
public:
	virtual void example() = 0;
};

class SampleFactory
{
public:
	static ThridLibSample* create(const char* libName);
};