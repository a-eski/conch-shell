#ifndef shl_debug_h
#define shl_debug_h

#include <stdio.h>

#include "shl_types.h"

void shl_debug_line(struct shl_Line* line);
void shl_debug_args(struct shl_Lines* args);
void shl_debug_launch_process(struct shl_Lines* args);

#endif // !shl_debug_h

