#include "gtest/gtest.h"
#include "cq_log.h"

TEST(CQLog, logTest)
{
	CQLog log;
	log << CQLog::INFO << "info color" << endl; // color: white
	log << "default color" << endl; // type: info, color: white

	log << CQLog::WARNING << "warning color" << endl;
	log << "default color" << endl; // type: info, color white

	log << CQLog::FAULT << "fault color" << endl; // color yellow

	log.setLogFilter(CQLog::FATAL);
	log << CQLog::FAULT << "should not be shown" << endl; // color yellow
	log << CQLog::FATAL << "fatal color" << endl; // color red

	log.log2File(L"log.txt");
	log << CQLog::WARNING << "write this log to file" << endl;
	log.closeLogFile();
}
