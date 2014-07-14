#pragma once

namespace common
{
#define Between(a, b, c) ((b) <= (a) && (a) <= (c)) //a ¡Ê [b, c]

template<typename Iterator, class InstanceType, class Function>
inline Function cm_for_each(Iterator first, Iterator last, InstanceType* instance, Function func)
{
	for (; first != last; ++first)
		func(instance, *first);
	return (func);
}

template<typename Iterator, class Function>
inline Function cm_for_each(Iterator first, Iterator last, Function func)
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

template<typename Type>
bool isOneOf(Type match, size_t listLen, ...)
{
	va_list pvar;
	va_start(pvar, num);
	for (size_t j = 0; j < listLen; j++)
	{
		if (match == va_arg(pvar, Type))
		{
			va_end(pvar);
			return true;
		}

	}
	va_end(pvar);
	return false;
}

template<class Iterator,
class Process> inline
	Iterator getFirstRepeat(Iterator first, Iterator last, Process proc)
{
	for(; first != last; first++){
		if (proc(*first)){
			break;
		}
	}
	return first;
}

vector<wstring> split(const wchar_t* str, const wchar_t* limit);
vector<string> split(const char* str,const char* limit);

}
