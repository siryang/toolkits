#include <stdio.h>

typedef enum EnumType
{
	EnumType_a,
	EnumType_b = 200
} EnumType;

int main()
{
	int m = (-8) % 5;
	printf("a:%d\n", EnumType_a);
	printf("b:%d\n", EnumType_b);
	printf("%d\n", m);
	printf("hello world\n");
	return 0;
}