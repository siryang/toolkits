#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>

// write logs to file.
using namespace std;

class CQLog
{
public:
	enum CQLogLevel
	{
		INFO,
		WARNING,
		FAULT, // ERROR is defined in window.h
		FATAL,
		maxLevel
	};

	typedef std::basic_ostream<char, std::char_traits<char>> CoutType;
	typedef CoutType& (*StandardEndLine)(CoutType&);
	typedef WORD WinCoutColor;

	CQLog(){
		m_fname[0] = 0;
		m_filterLevel = INFO;
	}

	CQLog(const wchar_t* fname){
		m_filterLevel = INFO;
		log2File(fname);
	}

	~CQLog(){
		closeLogFile();
	}

	void log2File(const wchar_t* fname);
	void closeLogFile();

	void setLogLevel(CQLogLevel level);
	bool levelFilter(){
		return m_currentLevel >= m_filterLevel;
	}

	template<class T>
	inline CQLog& operator << (T data)
	{
		if (!levelFilter()){
			return *this;
		}
		cout << data;
		if (m_ofs.is_open()){
			m_ofs << data;
		}
		return *this;
	}
	
	template<>
	inline CQLog& operator << (CQLogLevel level){
		setLogLevel(level);
		return *this;
	}

	template<>
	inline CQLog& operator << (wchar_t* data){
		return showWideString(data);
	}

	template<>
	inline CQLog& operator << (const wchar_t* data){
		return showWideString(data);
	}

	CQLog& operator<< (StandardEndLine sln);

private:
	CQLog& showWideString(const wchar_t* str);

	wchar_t m_fname[MAX_PATH];
	std::ofstream m_ofs;
	CQLogLevel m_currentLevel;
	CQLogLevel m_filterLevel;

	const static WinCoutColor s_colorTable[maxLevel];
};

