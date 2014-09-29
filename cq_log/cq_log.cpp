#include "cq_log.h"

const static CQLog::WinCoutColor blue = FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY;
const static CQLog::WinCoutColor red = FOREGROUND_RED|FOREGROUND_INTENSITY;
const static CQLog::WinCoutColor green = FOREGROUND_GREEN|FOREGROUND_INTENSITY;
const static CQLog::WinCoutColor yellow = FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY;
const static CQLog::WinCoutColor white = FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE;

const CQLog::WinCoutColor CQLog::s_colorTable[CQLog::maxLevel] = {
	white,
	blue,
	yellow,
	red
};

void CQLog::log2File( const wchar_t* fname )
{
	closeLogFile();
	wcscpy(m_fname, fname);
	m_ofs.open(fname, ios_base::app);
}

void CQLog::setLogLevel( CQLogLevel level )
{
	if (level != m_currentLevel)
	{
		m_currentLevel = level;
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
		SetConsoleTextAttribute(hStdout, s_colorTable[level]);
	}
}

CQLog& CQLog::operator<<( StandardEndLine sln )
{
	if (!levelFilter()){
		return *this;
	}

	sln(std::cout);

	if (m_ofs.is_open()){
		sln(m_ofs);
	}
	setLogLevel(INFO);
	return *this;
}

CQLog& CQLog::showWideString( const wchar_t* str )
{
	if (!levelFilter()){
		return *this;
	}
	char temp[1024];
	WideCharToMultiByte(CP_OEMCP, 0, str, -1, temp, 128, NULL, false);
	return operator << (temp);
}

void CQLog::closeLogFile()
{
	if (m_ofs.is_open()){
		m_ofs.close();
	}
}
