#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

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

struct shl_Line shl_read_line(void)
{
	uint_fast32_t bufferSize = SHL_BUFFER_SIZE;
	uint_fast32_t position = 0;
	char* buffer = malloc(sizeof(char) * bufferSize);
	int character;

	if (!buffer)
		exit(-1);

	while (1)
	{
		character = getchar();

		/* If character is EOF or newline, replace with null terminator and return */
		if (character == EOF || character == '\n')
		{
			buffer[position++] = '\0';
			struct shl_Line line = { .length = position, .line = buffer };
			return line;
		}
		else
		{
			buffer[position++] = character;
		}

		if (position > bufferSize)
		{
			bufferSize += SHL_BUFFER_SIZE;
			if (bufferSize >= SHL_MAX_BUFFER_SIZE)
			{	
				free(buffer);
				struct shl_Line emptyLine = { .length = 0, .line = NULL };
				return emptyLine;
			}

			buffer = realloc(buffer, bufferSize);
			if (!buffer)
				exit(-1);
		}

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
		if (character == EOF || character == '\n')
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

struct shl_Line shl_read_stream(void)
{
	uint_fast32_t bufferSize = SHL_BUFFER_SIZE;
	char buffer[bufferSize];
	uint_fast32_t position = 0;
	int character;

	while (1)
	{
		character = getchar();

		/* If character is EOF or newline, replace with null terminator and return */
		if (character == EOF || character == '\n')
		{
			buffer[position++] = '\0';
			struct shl_Line line = { .length = position, .line = buffer };
			return line;
		}
		else
		{
			buffer[position++] = character;
		}
	}
}

_Bool shl_is_valid_line(struct shl_Line line)
{
	if (line.length == 1)
		return false;

	if (line.line == NULL)
		return false;
	
	return true;
}

struct shl_Lines shl_split_line(struct shl_Line line)
{
	if (!shl_is_valid_line(line))
	{
		struct shl_Lines emptyLines = { .count = 0, .lines = NULL };
		return emptyLines;
	}

	uint_fast32_t bufferPosition = 0;
	uint_fast32_t bufferSize = SHL_TOKEN_BUFFER_SIZE;
	char* buffer = malloc(sizeof(char) * bufferSize);
	if (buffer == NULL)
		exit(-1);

	struct shl_Lines splitLines = { .count = 0, .lines = NULL };
	splitLines.lines = malloc(sizeof(char*) * SHL_TOKEN_BUFFER_SIZE);
	if (splitLines.lines == NULL)
		exit(-1);

	uint_fast32_t doubleQuotesCount = 0;

	for (uint_fast32_t linePosition = 0; linePosition < line.length + 1; linePosition++)
	{
		if (linePosition == line.length)
		{
			splitLines.lines[splitLines.count] = NULL;
		}
		else if (shl_is_delimiter(line.line[linePosition]) && (doubleQuotesCount == 0 || doubleQuotesCount == 2))
		{
			buffer[bufferPosition] = '\0';

			splitLines.lines[splitLines.count] = malloc(sizeof(char) * bufferSize);
			buffer = shl_string_copy(splitLines.lines[splitLines.count], buffer, bufferSize);
			splitLines.count++;

			if (splitLines.maxLineSize == 0 || bufferPosition > splitLines.maxLineSize)
				splitLines.maxLineSize = bufferPosition;

			buffer[0] = '\0';
			bufferPosition = 0;
			doubleQuotesCount = 0;
		}
		else
		{
			if (bufferPosition > bufferSize)
			{
				bufferSize += SHL_TOKEN_BUFFER_SIZE;
				if (bufferSize >= SHL_MAX_TOKEN_BUFFER_SIZE)
				{
					free(buffer);
					free(splitLines.lines);
					struct shl_Lines emptyLines = { .count = 0, .lines = NULL };
					return emptyLines;
				}

				buffer = realloc(buffer, bufferSize);
				if (!buffer)
					exit(-1);
			}
			
			if (line.line[linePosition] == '\"')
				doubleQuotesCount++;
			else	
				buffer[bufferPosition++] = line.line[linePosition];
		}
	}

	free(buffer);
	return splitLines;
}

_Bool shl_is_valid_args(struct shl_Lines args)
{
	if (args.count == 0 || args.lines == NULL)
		return 0;

	return 1;
}

