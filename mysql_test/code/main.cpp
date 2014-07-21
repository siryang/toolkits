#include <mysql.h>
#include <iostream>

using namespace std;

#ifdef _WIN
#include <WinSock2.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

static char *server_args[] = {
	"this_program",       /* this string is not used */
	"--datadir=.",
	"--key_buffer_size=32M"
};

static char *server_groups[] = {
	"embedded",
	"server",
	"this_program_SERVER",
	(char *)NULL
};
#define SHOW_STMT_ERRROR(trace) {cout << (trace) << __LINE__ << endl;\
	cout << mysql_stmt_error(insertStmt) << mysql_stmt_errno(insertStmt);}

#define SHOW_MYSQL_ERROR(trace) {cout << (trace) << __LINE__ << endl;\
	cout << mysql_error(m_mysql) << mysql_errno(m_mysql);}


class MysqlConntector
{
public:
	MysqlConntector(const char *host,
		const char *user,
		const char *passwd,
		const char *db,
		unsigned int port,
		const char *unix_socket,
		unsigned long clientflag)
	{
		char *server_options[] = { "mysql_test", "--defaults-file=my.cnf" };
		int num_elements = sizeof(server_options)/ sizeof(char *);

		if (mysql_server_init(sizeof(server_args) / sizeof(char *), server_args, server_groups))
			exit(1);

		m_mysql = mysql_init(NULL);
		mysql_options(m_mysql, MYSQL_READ_DEFAULT_GROUP, "libmysqld_client");
		mysql_options(m_mysql, MYSQL_OPT_USE_EMBEDDED_CONNECTION, NULL);

		if (!mysql_real_connect(m_mysql, "192.168.0.162", "comment", "comment", "test", 3306, NULL ,0))
		{
			SHOW_MYSQL_ERROR("Init mysql connection fail!");
			exit(1);
		}
	}

	void testInsert()
	{
		MYSQL_STMT* insertStmt;
		int key = 0; 
		char buffer[4096];
		char insertCommand[256] = "INSERT INTO test VALUES(?,?)";
		
		insertStmt = mysql_stmt_init(m_mysql);
		if (mysql_stmt_prepare(insertStmt, insertCommand, strlen(insertCommand)) != 0)
		{
			SHOW_MYSQL_ERROR("Init mysql connection fail!");
			SHOW_STMT_ERRROR("Insert route result fail!");
		}
		mysql_query(m_mysql, "START TRANSACTION;");

		//for (int i = 0; i < 10000; i++)
		{
			for (int j = 0; j < 1000; j++)
			{
				my_bool isNull = false;
				unsigned long lengthInt = 4, lengthBlob = 4096;
				MYSQL_BIND bind[2];

				bind[0].buffer_type = MYSQL_TYPE_LONG;
				bind[0].buffer = &(++key);
				bind[0].is_null = &isNull;

				bind[1].buffer_type = MYSQL_TYPE_BLOB;
				bind[1].buffer = buffer;
				bind[1].buffer_length = lengthBlob;
				bind[1].is_null = &isNull;

				mysql_stmt_bind_param(insertStmt, bind);
				if (mysql_stmt_execute(insertStmt) != 0)
				{
					cout << "exec fail" << endl;
				}
			}
		}
		mysql_stmt_close(insertStmt);
		mysql_query(m_mysql, "END TRANSACTION;");
	}

	void testSelect()
	{
		MYSQL_RES *results;
		MYSQL_ROW record;

		mysql_query(m_mysql, "SELECT * FROM test WHERE id=\"333\"");

		results = mysql_store_result(m_mysql);

		while((record = mysql_fetch_row(results))) 
		{
			size_t i;
			for (i = 0; i < results->field_count; i++)
			{
				printf("%s%s", record[i], i == results->field_count - 1? "\n": " - ");
			}
		}
	}

	~MysqlConntector()
	{
		mysql_close(m_mysql);
		mysql_server_end();
	}
private:
	MYSQL *m_mysql;
};

int main()
{
	MysqlConntector connector("127.0.0.1", "test", "test", "test", 3306, NULL, 0);
	connector.testInsert();
	return 0;
}