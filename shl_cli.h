#ifndef shl_cli_h
#define shl_cli_h

#include <stdbool.h>
#include <stdint.h>
#include <linux/limits.h>

#define SHL_BUFFER_SIZE 4096
#define SHL_MAX_BUFFER_SIZE 20480

#define SHL_TOKEN_BUFFER_SIZE 64
#define SHL_MAX_TOKEN_BUFFER_SIZE 1280
#define SHL_LINES_SIZE 10

struct shl_Line
{
	uint_fast32_t length;
	char* line;
};

struct shl_Args
{
	uint_fast32_t count;
	uint_fast32_t maxLineSize;
	char** lines;
};

struct shl_Directory
{
	char path[PATH_MAX];
};

_Bool shl_is_delimiter(char ch);

struct shl_Line shl_line_read(void);
_Bool shl_line_is_valid(struct shl_Line line);

struct shl_Args shl_line_split(struct shl_Line line);
_Bool shl_args_is_valid(struct shl_Args args);

#endif // !shl_cli_h

