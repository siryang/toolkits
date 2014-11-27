#include "ass_math.h"
#include <cstring>

#define INT_DIGITS 19		/* enough for 64 bit integer */
void ass_itoa64(int64 i, char* s, int radix)
{
	/* Room for INT_DIGITS digits, - and '\0' */
	static char buf[INT_DIGITS + 2];
	char *p = buf + INT_DIGITS + 1;	/* points to terminating '\0' */
	if (i >= 0) {
		do {
			*--p = '0' + (char)(i % radix);
			i /= radix;
		} while (i != 0);
	}
	else {			/* i < 0 */
		do {
			*--p = '0' - (char)(i % radix);
			i /= radix;
		} while (i != 0);
		*--p = '-';
	}
	strcpy(s, p);
}

int64 ass_atoi64(const char* str)
{
	int64 re = 0;

	for (; *str != 0; str++)
	{
		re = re * 10 + (*str) - '0';
	}

	return re;
}