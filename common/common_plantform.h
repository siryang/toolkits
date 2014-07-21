#pragma once

/**
	用于window和linux兼容的接口
*/

#if defined(_WIN32) || defined(_WIN64)
#include < time.h >
#include <WinSock.h>
struct timeval;
struct timezone;
int gettimeofday(struct timeval *tv, struct timezone *tz);

# define timersub(a, b, result)								\
	do {													\
	(result)->tv_sec = (a)->tv_sec - (b)->tv_sec;		\
	(result)->tv_usec = (a)->tv_usec - (b)->tv_usec;	\
	if ((result)->tv_usec < 0)							\
{													\
	--(result)->tv_sec;								\
	(result)->tv_usec += 1000000;					\
}													\
	} while (0)

# define timeradd(a, b, result)								\
	do {													\
	(result)->tv_sec = (a)->tv_sec + (b)->tv_sec;		\
	(result)->tv_usec = (a)->tv_usec + (b)->tv_usec;	\
	if ((result)->tv_usec >= 1000000)					\
{													\
	++(result)->tv_sec;								\
	(result)->tv_usec -= 1000000;					\
}													\
	} while (0)


#else
#include <time.h>
#include <sys/time.h>

#endif 