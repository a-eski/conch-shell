#ifndef shl_types_h
#define shl_types_h

#include <stdint.h>

struct shl_Line
{
	uint_fast32_t length;
	char* line;
};

struct shl_Lines
{
	uint_fast32_t count;
	uint_fast32_t maxLineSize;
	char** lines;
};

#endif // !shl_types_h

