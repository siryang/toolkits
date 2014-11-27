#pragma once

#include "ass_type.h"

void ass_itoa64(int64 i, char* s, int radix);
int64 ass_atoi64(const char* str);

int ass_atoi_hex()