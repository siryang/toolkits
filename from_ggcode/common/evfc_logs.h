#pragma once
#include <iostream>
#include <fstream>
#include <string>

// write logs to file.
class CQLog
{
public:
	CQLog(const wchar_t* fname)
	{
		m_fname = fname;
		m_ofs.open(fname);
	}
	~CQLog()
	{
		m_ofs.close();
	}

	template<class T>
	CQLog& operator << (T data)
	{
		cout << data;
		m_ofs << data;
		return *this;
	}

	typedef std::basic_ostream<char, std::char_traits<char>> CoutType;
	typedef CoutType& (*StandardEndLine)(CoutType&);
	CQLog& operator<< (StandardEndLine sln)
	{
		sln(std::cout);
		sln(m_ofs);
		return *this;
	}
private:
	std::wstring m_fname;
	std::ofstream m_ofs;
};


