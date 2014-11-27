#include "third_lib_factory.h"
#include "libcurl/libcurl_simple.h"
#include "pthread_simple/thread_example.h"
#include "mysql_conn_pool/mysql_conn_example.h"

ThridLibSample* SampleFactory::create( const char* libName )
{
	if (strcmp(libName, "libcurl") == 0)
	{
		return new LibCurlSample;
	}
	else if (strcmp(libName, "thread_pool") == 0)
	{
		return new ThreadPoolSample;
	}
	else if (strcmp(libName, "mysql_conn_pool") == 0)
	{
		return new MysqlConnPoolSample;
	}
	else if (strcmp(libName, "simple_thread_pool") == 0)
	{
		return new ThreadPoolSimpleExample;
	}
	else
	{
		return NULL;
	}
}
