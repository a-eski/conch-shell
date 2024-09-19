#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "shl_string.h"
#include "shl_cli.h"

_Bool shl_is_delimiter(char ch)
{
	switch (ch)
	{
		case ' ':
			return true;
		case '\t':
			return true;
		case '\r':
			return true;
		case '\n':
			return true;
		case '\a':
			return true;
		case EOF:
			return true;
		case '\0':
			return true;
		default:
			return false;
	}
}

struct shl_Line shl_line_read(void)
{
	char buffer[SHL_BUFFER_SIZE];
	uint_fast32_t position = 0;
	int character;

	while (1)
	{
		character = getchar();
		
		/* If character is EOF or newline, replace with null terminator and return */
		if (character == EOF || character == '\n' || position == SHL_BUFFER_SIZE - 1)
		{
			buffer[position++] = '\0';
			char* result = malloc(sizeof(char) * position);
			shl_string_copy(result, buffer, position);
			struct shl_Line line = { .length = position, .line = result };
			return line;
		}
		else
		{
			buffer[position++] = character;
		}
	}
}

_Bool shl_line_is_valid(struct shl_Line line)
{
	if (line.length == 1)
		return false;

	if (line.line == NULL)
		return false;
	
	return true;
}

struct shl_Args shl_line_split(struct shl_Line line)
{
	const uint_fast32_t buffer_size = SHL_TOKEN_BUFFER_SIZE;
	char buffer[SHL_TOKEN_BUFFER_SIZE];
	uint_fast32_t buffer_position = 0;

	struct shl_Args args = { .count = 0, .lines = NULL };
	args.lines = malloc(sizeof(char*) * SHL_TOKEN_BUFFER_SIZE);
	if (args.lines == NULL)
		exit(-1);

	uint_fast32_t double_quotes_count = 0;

	for (uint_fast32_t line_position = 0; line_position < line.length + 1; line_position++)
	{
		if (line_position == line.length || buffer_position == SHL_TOKEN_BUFFER_SIZE - 1)
		{
			args.lines[args.count] = NULL;
			break;
		}
		else if (shl_is_delimiter(line.line[line_position]) && (double_quotes_count == 0 || double_quotes_count == 2))
		{
			buffer[buffer_position] = '\0';

			args.lines[args.count] = malloc(sizeof(char) * buffer_size);
			shl_string_copy(args.lines[args.count], buffer, buffer_size);
			args.count++;

			if (args.maxLineSize == 0 || buffer_position > args.maxLineSize)
				args.maxLineSize = buffer_position;

			buffer[0] = '\0';
			buffer_position = 0;
			double_quotes_count = 0;
		}
		else
		{
			if (line.line[line_position] == '\"')
				double_quotes_count++;
			else	
				buffer[buffer_position++] = line.line[line_position];
		}
	}

	return args;
}

_Bool shl_args_is_valid(struct shl_Args args)
{
	if (args.count == 0 || args.lines == NULL)
		return 0;

	return 1;
}

