#define GOOGLE_GLOG_DLL_DECL

#include "gtest/gtest.h"
#include "glog/logging.h"

int main(int argc, char* argv[]) 
{
	// Initialize Google's logging library.
	google::InitGoogleLogging(argv[0]);
	FLAGS_log_dir = "../../third_library_test/googlelog";  
	// ...
	LOG_IF(WARNING, true) << "log_if this";
	LOG_IF(WARNING, false) << "don't log_if this";

	LOG(INFO) << "Found " << 3 << " cookies";
	LOG(WARNING) << "Warning" << 4;
	LOG(ERROR) << "Warning" << 5;
	//LOG(FATAL) << "Warning" << 6;
	system("pause");
	return 0;
}