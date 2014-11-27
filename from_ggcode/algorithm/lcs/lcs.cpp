#include "lcs.h"

// �������lcs��ʹ�õݹ���㣬��ʵ�õ��㷨���ڴ˻����ϵ��Ż���
string getLCS(const string& string1, const string& string2)
{
	string result;
	size_t len1 = string1.length();
	size_t len2 = string2.length();
	if (len1 == 0 || len2 == 0)
	{
		return result;
	}
	
	string newString1 = string1;
	string newString2 = string2;
	newString1.resize(len1 - 1);
	newString2.resize(len2 - 1);

	if (string1.at(len1 - 1) == string2.at(len2 - 1))
	{
		result = getLCS(newString1, newString2);	
		result.push_back(string1.at(len1 - 1));
	}
	else
	{
		string result1, result2;
		result1 = getLCS(string1, newString2);
		result2 = getLCS(newString1, string2);
		result = result1.length() > result2.length()? result1: result2;
	}
	return result;
}