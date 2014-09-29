#include "gtest/gtest.h"
#include "cq_log.h"

TEST(CQLog, logTest)
{
	CQLog log;
	log << CQLog::INFO << "info" << endl; // color white
	log << "info" << endl; // color white

	log << CQLog::WARNING << "warning" << endl;
	log << "info" << endl; // color white

	log << CQLog::FAULT << "fault" << endl; // color yellow
	log << CQLog::FATAL << "fatal" << endl; // color red

	log.log2File(L"");
}