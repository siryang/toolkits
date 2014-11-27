#include "third_lib_factory.h"

class SampleFactory;
int main()
{
	//ThridLibSample* simple = SampleFactory::create("thread_pool");
	//if (simple != NULL)
	//{
	//	simple->example();
	//}
	//delete simple;

	ThridLibSample* simple = SampleFactory::create("simple_thread_pool");
	if (simple != NULL)
	{
		simple->example();
	}
	delete simple;


	//ThridLibSample* simple = SampleFactory::create("mysql_conn_pool");
	//if (simple != NULL)
	//{
	//	simple->example();
	//}
	//delete simple;


	cout << "Hello" << endl;
	system("pause");
	return 0;
}

