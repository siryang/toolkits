#include "common_algorithm.h"

namespace common
{
	using namespace  std;
	vector<wstring> split(const wchar_t* str,const wchar_t* c)
	{
		wchar_t *cstr, *p, *token;
		vector<wstring> res;
		token = cstr = new wchar_t[wcslen(str) + 1];
		wcscpy(cstr, str);
		while((p = wcstok_s(NULL, c, &token)) != NULL)
		{
			res.push_back(p);
		}
		delete[] cstr;
		return res;
	}


	vector<string> split(const char* str,const char* c)
	{
		char *cstr, *p, *token;
		vector<string> res;
		token = cstr = new char[strlen(str) + 1];
		strcpy(cstr,str);
		while((p = strtok_s(NULL, c, &token)) != NULL)
		{
			res.push_back(p);
		}
		delete[] cstr;
		return res;
	}

}