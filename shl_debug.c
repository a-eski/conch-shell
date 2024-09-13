#include <stdint.h>

#include "shl_debug.h"

void shl_debug_line(struct shl_Line* line)
{
	printf("line.line: %s\n", line->line);
	printf("line.length: %lu\n", line->length);
}

void shl_debug_args(struct shl_Lines* args)
{
	printf("args.count: %lu\n", args->count);
	printf("args.maxLineSize: %lu\n", args->maxLineSize);
	for (uint_fast32_t i = 0; i < args->count; i++)
		printf("args[%lu] %s\n", i, *(args->lines + i));
}

