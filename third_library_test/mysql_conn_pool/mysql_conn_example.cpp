#include "mysql_conn_example.h"
#include "resource_pool.h"
//
//#define MYSQL_SERVER_ADDRESS "192.168.0.162"
//#define MYSQL_SERVER_PORT 3306
//#define MYSQL_SERVER_USER "comment"
//#define MYSQL_SERVER_PASSWORD "comment"
//#define MYSQL_SERVER_DATABASE "test"
//#define MYSQL_SERVER_ROUTE_RESULT_TABLE "route_c_ware"
//
//static char *server_args[] = {
//	"this_program",       /* this string is not used */
//	"--datadir=.",
//	"--key_buffer_size=32M"
//};
//static char *server_groups[] = {
//	"embedded",
//	"server",
//	"this_program_SERVER",
//	(char *)NULL
//};
//
//
//class MysqlConnection
//{
//public:
//	// init connection
//	MysqlConnection();
//	~MysqlConnection();
//	
//	bool select(const char* cmd);
//	bool delete(const char* cmd);
//
//private:
//	MYSQL* mysql;
//	MYSQL_STMT* mysqlStmt;
//};
//
//MysqlConnection::MysqlConnection()
//{
//	my_bool tempBool = 1;
//	// select
//	mysql = mysql_init(NULL);
//
//	mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "libmysqld_client");
//	mysql_options(mysql, MYSQL_OPT_USE_EMBEDDED_CONNECTION, NULL);
//
//	if (!mysql_real_connect(mysql, MYSQL_SERVER_ADDRESS , MYSQL_SERVER_USER, MYSQL_SERVER_PASSWORD
//		, MYSQL_SERVER_DATABASE, MYSQL_SERVER_PORT, NULL ,0))
//	{
//		perror("Init mysql connection fail!\n");
//		goto init_fail;
//	}
//
//	mysqlStmt = mysql_stmt_init(mysql);
//	if (mysqlStmt == NULL)
//	{
//		perror("Init mysql stmt fail!\n");
//		goto init_fail;
//	}
//
//	mysql_stmt_attr_set(mysqlStmt, STMT_ATTR_UPDATE_MAX_LENGTH, (void*)&tempBool);
//	return;
//
//init_fail:
//	mysql_close(mysql);
//	exit(1);
//	return;
//}
//
//MysqlConnection::~MysqlConnection()
//{
//	mysql_stmt_close(mysqlStmt);
//	mysql_close(mysql);
//}
//
//const static int connNum = 10;
//typedef ResourcePool<MysqlConnection> MysqlConnPool;
typedef ResourcePool<int> IntResource;

void MysqlConnPoolSample::example()
{
	//MysqlConnection* conns = new MysqlConnection[connNum];
	//MysqlConnPool::instance()->addResource(conns, connNum);
	IntResource::instance();

	//delete conns;
}
