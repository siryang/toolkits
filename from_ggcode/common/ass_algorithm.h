#pragma once

template<typename Iterator, class InstanceType, class Function>
inline Function sr_for_each(Iterator first, Iterator last, InstanceType* instance, Function func)
{
	for (; first != last; ++first)
		func(instance, *first);
	return (func);
}

template<typename Iterator, class Function>
inline Function sr_for_each(Iterator first, Iterator last, Function func)
{
	for (; first != last; ++first)
		func(*first);
	return (func);
}

template<typename Iterator, class InstanceType, class Filter, class Procer>
inline Iterator filter(Iterator first, Iterator last, InstanceType* instance, Function filter, Procer proc)
{
	for (; first != last; ++first)
	{
		if (filter(instance, *first))
			proc(instance, *first);
	}
}