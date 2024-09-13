#ifndef shl_cli_h
#define shl_cli_h

#include <stdbool.h>

#include "shl_types.h"

#define SHL_BUFFER_SIZE 1024
#define SHL_MAX_BUFFER_SIZE 20480
#define SHL_TOKEN_BUFFER_SIZE 64
#define SHL_MAX_TOKEN_BUFFER_SIZE 1280
#define SHL_LINES_SIZE 10

_Bool shl_is_delimiter(char ch);

struct shl_Line shl_read_line(void);
_Bool shl_is_valid_line(struct shl_Line* line);

struct shl_Lines shl_split_line(struct shl_Line* line);
_Bool shl_is_valid_args(struct shl_Lines* args);

#endif // !shl_cli_h

