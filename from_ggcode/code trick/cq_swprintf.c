#include <stdarg.h>
#include "cq_types_basic.h"

#define cqFORMAT_INT      0    //整型
#define cqFORMAT_STRING   1    //字符串型
#define cqFORMAT_CHAR     2    //字符型
#define cqFORMAT_HEX      3    //16进制
#define cqFORMAT_LONG     4    //长整型
#define cqFORMAT_UNSIGNED 5    //无符号整型
#define cqFORMAT_FLOAT    6    //浮点型
#define cqFORMAT_HEX_UPPERCASE      7
#define cqFORMAT_ANTISTRING   8    // 和当前定义相反的字符串。printf中，就是Unicode字符串。wprintf中，就是单字节的。

#define _isDigit(o) ((o) >= '0' && (o) <= '9')

static int32 cq_read_format_stringw(cqWCHAR **ps, size_t* width, cqCHAR* padding)
{
	*width = 0;
	(*ps)++;
	*padding = ' ';

	if (_isDigit(**ps))
	{
		if (**ps == '0')
			*padding = '0';
		else
			*padding = ' ';
		*width = cq_wtoi(*ps);
		while(_isDigit(**ps))
		{
			(*ps)++;
		}
	}

	if(**ps == 'd' || **ps == 'l')
	{
		(*ps)++;
		return cqFORMAT_INT;
	}
	else if(**ps == 's')
	{
		(*ps)++;
		return cqFORMAT_STRING;
	}
	else if (**ps == 'S')
	{
		(*ps)++;
		return cqFORMAT_ANTISTRING;
	}
	else if(**ps == 'c')
	{
		(*ps)++;
		return cqFORMAT_CHAR;
	}
	else if(**ps == 'x')
	{
		(*ps)++;
		return cqFORMAT_HEX;
	}
	else if(**ps == 'X')
	{
		(*ps)++;
		return cqFORMAT_HEX_UPPERCASE;
	}
	else if(**ps == 'u')
	{
		(*ps)++;
		if(**ps == 'l')
			(*ps)++;
		return cqFORMAT_UNSIGNED;
	}

	return cqFORMAT_INT;
}

int32 cq_swprintf2(cqWCHAR *buffer, const cqWCHAR *format , ...)
{
	cqWCHAR *p = buffer;
	cqWCHAR *q = (cqWCHAR *)format;
	va_list vl;
	size_t width;
	int32 type = cqFORMAT_INT;
	int32 intVal;
	// float64 fVal;
	size_t len = 0;
	cqCHAR padding;

	va_start(vl, format);

	while(*q != 0)
	{
		if(*q == '%') //格式符
		{
			type = cq_read_format_stringw(&q, &width, &padding);
			switch(type)
			{
			case cqFORMAT_INT:
			case cqFORMAT_HEX:
			case cqFORMAT_HEX_UPPERCASE:
			case cqFORMAT_UNSIGNED:
				{
					cqWCHAR s1[32];
					size_t s1Len;

					intVal = (int32)va_arg(vl, int32);

					switch(type)
					{
					case cqFORMAT_INT:
						cq_itow(intVal, s1, 10);
						break;
					case cqFORMAT_HEX:
						cq_uitow(intVal, s1, 16);
						break;
					case cqFORMAT_HEX_UPPERCASE:
						cq_uitow(intVal, s1, 16);
						cq_wcsupr(s1);
						break;
					case cqFORMAT_UNSIGNED:
						cq_uitow(intVal, s1, 10);
						break;
					}

					s1Len = cq_wcslen(s1);
					if (s1Len < width)
					{
						size_t i;
						for(i = 0; i < width - s1Len; i++)
						{
							p[i] = padding;
						}
						memcpy(p + width - s1Len, s1, (s1Len + 1) * 2);
						if (type == cqFORMAT_INT && intVal < 0 && padding == '0')
						{
							swap(cqWCHAR, p[0], p[width - s1Len]);
						}
						
						len += width;
						p += width;
					}
					else
					{
						memcpy(p, s1, (s1Len + 1) * 2);
						len += s1Len;
						p += s1Len;
					}
				}
				break;
			case cqFORMAT_CHAR:
				*p++ = (cqWCHAR)va_arg(vl, int);
				len++;
				break;
			case cqFORMAT_STRING:
				{
					cqWCHAR *k = (cqWCHAR *)va_arg(vl, cqWCHAR *);
					while(*k != 0)
					{
						*p++ = *k++;
						len++;
					}
				}
				break;
			case cqFORMAT_ANTISTRING:
				{
					cqCHAR *k = (cqCHAR *)va_arg(vl, cqWCHAR *);
					while(*k != 0)
					{
						*p++ = *k++;
						len++;
					}
				}
				break;
			}
		}
		else
		{
			*p++ = *q++;
			len++;
		}
	}
	va_end(vl);
	*p = 0;
	return (int)len;
}
