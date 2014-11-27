#include <stdlib.h>
#include <stdio.h>
#include <string.h>




typedef struct vectorVV
{
	size_t m_reservedSize;
	size_t m_size;
	int* m_buffer;
} vectorVV;

void vectorVV_construct(vectorVV* o)
{
	o->m_reservedSize = 0;
	o->m_size = 0;
	o->m_buffer = NULL;
}

void vectorVV_reserve(vectorVV* o, size_t newSize) // 0xBFFF F2DO 0XBFFF F2d4
{
	// eax 0XBFFF F2EC 
	// esp 0xBFFF F2A0
	// ebp 0XBFFF F2C8 --> may be stack.
	
	// ebp + 8 == &o;
	// ebp + 12 == &newSize
	// ebp + 18 == 
	
	// eip   0x0804 847C (vectorVV_reserve 76 + current cmd pos)
	// mian  0x0804 84F1
	size_t index;	// 0XBFFF F2BC
	size_t mask;	// 0XBFFF F2B8
	int* newBuffer;

	if (newSize <= o->m_reservedSize)
		return;

	mask = newSize << 1;
	/*if (cq_bitScanReverse(&index, mask))
		newSize = (size_t)(1) << index;
	else
		return;
	*/
	newBuffer = (int*)malloc(newSize * sizeof(int));
	if (newBuffer == NULL)
	{
		return;
	}
	memcpy(newBuffer, o->m_buffer, o->m_reservedSize * sizeof(int));
	free(o->m_buffer);
	o->m_buffer = newBuffer;
	o->m_reservedSize = newSize;
}

void main()
{
	int a = 3;
	int b = 4;
	vectorVV p;
	vectorVV_construct(&p);
	vectorVV_reserve(&p, 32);
	a = a + b;
}
