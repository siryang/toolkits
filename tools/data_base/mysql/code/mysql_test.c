#ifdef WIN32
#pragma message("defined WIN32")
#include <WinSock2.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

const char* server = "192.168.0.162";
//const char* server = "10.10.22.60";

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
//char *server_options[] = { "mysql_test", "--defaults-file=my.cnf" };

void access_thread(void* args)
{
	mysql_thread_init();
	// call some api here.

	mysql_thread_end();
}

void getGuid(char* guidStr)
{
	GUID guid;
	wchar_t buffer[90];
	CoCreateGuid(&guid);
	StringFromGUID2(&guid, buffer, 90);
	wcstombs(guidStr, buffer, 80);
}

typedef struct StructSample
{
	int a;
	int b;
	int c;
} StructSample;
#define STRING_SIZE 50

void testStmtSelect()
{
#define SELECT_SAMPLE "SELECT id, request, segments FROM route_c_ware"
	MYSQL* mysql;
	MYSQL_RES     *prepare_meta_result;
	MYSQL_STMT    *stmt;
	MYSQL_BIND	  bindSearch[1];
	MYSQL_BIND    bind[5];
	MYSQL_TIME    time;
	my_ulonglong  affected_rows;
	int           param_count;
	short         small_data;
	int           int_data;
	char          str_data[STRING_SIZE] = "{C78B7C4D-50B7-4D95-8CB6-574E823D7182}";
	StructSample  struct_sample = {1,2,3};
	unsigned long str_length;
	unsigned long segments_length;
	my_bool       is_null;

	char request[256];
	char segments[256];
	request[0] = 0;
	segments[0] = 0;

	mysql = mysql_init(NULL);
	mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "libmysqld_client");
	mysql_options(mysql, MYSQL_OPT_USE_EMBEDDED_CONNECTION, NULL);

	mysql_real_connect(mysql, server, "comment", "comment", "route_c", 3306, NULL ,0);
	
	stmt = mysql_stmt_init(mysql);
	if (!stmt)
	{
		fprintf(stderr, " mysql_stmt_init(), out of memory\n");
		exit(0);
	}

	mysql_stmt_prepare(stmt, SELECT_SAMPLE, strlen(SELECT_SAMPLE));
	param_count= mysql_stmt_param_count(stmt);
	if (param_count != 0)
	{
		printf(mysql_stmt_error(stmt));
		return;
	}
	memset(bindSearch, 0, sizeof(bindSearch));
	bindSearch[0].buffer_type= MYSQL_TYPE_STRING;
	bindSearch[0].buffer= (char*)str_data;
	bindSearch[0].is_null= 0;
	bindSearch[0].buffer_length = strlen(str_data);

	prepare_meta_result = mysql_stmt_result_metadata(stmt);
	if (!prepare_meta_result)
	{
		return;
	}

	if (mysql_num_fields(prepare_meta_result)!= 3) /* validate column count */
	{
		fprintf(stderr, " invalid column count returned by MySQL\n");
		exit(0);
	}

	mysql_stmt_bind_param(stmt, bindSearch);
	mysql_stmt_execute(stmt);
	memset(&struct_sample, 0, sizeof(StructSample));
	memset(bind, 0, sizeof(bind));
	// route id 
	str_data[0] = 0;
	bind[0].buffer_type= MYSQL_TYPE_STRING;
	bind[0].buffer = (char*)str_data;
	bind[0].buffer_length = STRING_SIZE;
	bind[0].is_null= &is_null;
	bind[0].length= &segments_length;
	// request
	bind[1].buffer_type= MYSQL_TYPE_STRING;
	bind[1].buffer= (char*)request;
	bind[1].buffer_length = sizeof(request);

	// tmc time
	bind[2].buffer_type= MYSQL_TYPE_TIMESTAMP;
	bind[2].buffer= (char*)&time;
	bind[2].buffer_length = sizeof(time);

	// segments 
	bind[3].buffer_type= MYSQL_TYPE_TINY_BLOB;
	bind[3].buffer= (char*)segments;
	bind[3].buffer_length = sizeof(segments);
	bind[3].length = &str_length;

	if(mysql_stmt_bind_result(stmt, bind))
	{
		return;
	}
	mysql_stmt_fetch(stmt);

	mysql_stmt_close(stmt);
	mysql_close(mysql);
}
void testStmtInsert()
{

#define INSERT_SAMPLE "INSERT INTO route_c_ware VALUES(?,?,?,?,?)"
	MYSQL* mysql;
	MYSQL_STMT    *stmt;
	MYSQL_BIND    bind[5];
	
	MYSQL_TIME    time;

	my_ulonglong  affected_rows;
	int           param_count;
	short         small_data;
	int           int_data;
	char          str_data[STRING_SIZE];
	StructSample  struct_sample = {1,2,3};
	unsigned long str_length;
	my_bool       is_null;

	/* Prepare an INSERT query with 3 parameters */
	/* (the TIMESTAMP column is not named; the server */
	/*  sets it to the current date and time) */
	mysql = mysql_init(NULL);
	mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "libmysqld_client");
	mysql_options(mysql, MYSQL_OPT_USE_EMBEDDED_CONNECTION, NULL);

	mysql_real_connect(mysql, server, "comment", "comment", "test", 3306, NULL ,0);

	stmt = mysql_stmt_init(mysql);
	if (!stmt)
	{
		fprintf(stderr, " mysql_stmt_init(), out of memory\n");
		exit(0);
	}

	if (mysql_stmt_prepare(stmt, INSERT_SAMPLE, strlen(INSERT_SAMPLE)))
	{
		fprintf(stderr, " mysql_stmt_prepare(), INSERT failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		exit(0);
	}
	fprintf(stdout, " prepare, INSERT successful\n");

	/* Get the parameter count from the statement */
	param_count= mysql_stmt_param_count(stmt);
	fprintf(stdout, " total parameters in INSERT: %d\n", param_count);

	if (param_count != 5) /* validate parameter count */
	{
		fprintf(stderr, " invalid parameter count returned by MySQL\n");
		exit(0);
	}

	/* Bind the data for all 3 parameters */

	memset(bind, 0, sizeof(bind));

	/* INTEGER PARAM */
	/* This is a number type, so there is no need to specify buffer_length */
	// route id 
	getGuid(str_data);
	bind[0].buffer_type= MYSQL_TYPE_STRING;
	bind[0].buffer= (char*)str_data;
	bind[0].is_null= 0;
	bind[0].buffer_length = strlen(str_data);
	
	// request
	bind[1].buffer_type= MYSQL_TYPE_STRING;
	bind[1].buffer= "stmt this request";
	bind[1].buffer_length= strlen("stmt this request");
	bind[1].is_null= 0;
	bind[1].length= 0;

	// tmc time
	sscanf("2013-07-31 11:24:33", "%d-%d-%d %d:%d:%d", &time.year, &time.month, &time.day, &time.hour, &time.minute, &time.second);
	bind[2].buffer_type= MYSQL_TYPE_TIMESTAMP;
	bind[2].buffer= (char*)&time;
	//bind[2].buffer_length = ;
	bind[2].is_null= 0;
	bind[2].length= 0;

	// segments 
	bind[3].buffer_type= MYSQL_TYPE_STRING;
	bind[3].buffer= "1,2,3,4";
	bind[3].buffer_length= strlen("1,2,3,4");
	bind[3].is_null= 0;
	bind[3].length= 0;

	// test binary
	
	bind[4].buffer_type= MYSQL_TYPE_TINY_BLOB;
	bind[4].buffer= (char*)&struct_sample;
	bind[4].buffer_length = sizeof(StructSample);
	bind[4].is_null= 0;
	bind[4].length= 0;


	if (mysql_stmt_bind_param(stmt, bind))
	{
		fprintf(stderr, " mysql_stmt_bind_param() failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		exit(0);
	}

	/* Execute the INSERT statement - 1*/
	if (mysql_stmt_execute(stmt))
	{
		fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		exit(0);
	}

	/* Close the statement */
	if (mysql_stmt_close(stmt))
	{
		fprintf(stderr, " failed while closing the statement\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		exit(0);
	}
	mysql_close(mysql);
}

void testSendQuerySQL()
{
	UINT32 fieldCount;
	int queryState;
	MYSQL *mysql;
	MYSQL_RES *results;
	MYSQL_ROW record;
	char cmd[1024];
	char guidStr[80];

	getGuid(guidStr);

	mysql = mysql_init(NULL);
	mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "libmysqld_client");
	mysql_options(mysql, MYSQL_OPT_USE_EMBEDDED_CONNECTION, NULL);

	mysql_real_connect(mysql, server, "comment", "comment", "test", 3306, NULL ,0);
	sprintf(cmd, "INSERT INTO route_c_ware VALUES('%s', 'req', '2013-07-31 11:24:33', 'this is segment')", guidStr);
	// return 0 means success.
	queryState = mysql_query(mysql, cmd);
	printf("%d\n", queryState);

	queryState = mysql_query(mysql, "INSERT INTO route_c_ware VALUES('888', 'req', '2013-07-31 11:24:33', 'this is segment')");
	printf("%d\n", queryState);

	sprintf(cmd, "SELECT * FROM route_c_ware WHERE id='%s'", guidStr);

	queryState = mysql_query(mysql, "");
	printf("%d\n", queryState);

	results = mysql_store_result(mysql);
	fieldCount = mysql_field_count(mysql);
	while((record = mysql_fetch_row(results))) 
	{
		size_t i;
		for (i = 0; i < fieldCount; i++)
		{
			printf("%s%s", record[i], i == fieldCount - 1? "\n": " - ");
		}
	}

	mysql_free_result(results);
	mysql_close(mysql);
}

int main(void) 
{
	//int num_elements = sizeof(server_options)/ sizeof(char *);
	if (mysql_server_init(sizeof(server_args) / sizeof(char *), server_args, server_groups))
		exit(1);

	//testStmtInsert();
	testStmtSelect();
	mysql_server_end();

	return EXIT_SUCCESS;
}
