#include <assert.h>
#include <string.h>

#include "../shl_cli.h"
#include "../shl_types.h"

void shl_split_line_one_arg_test(void)
{
	struct shl_Line line = { .line = "ls\r", .length = 2 };
	struct shl_Lines args;

	args = shl_split_line(&line);
	
	assert(args.count == 1);
	assert(strcmp(args.lines[0], "ls") == 0);
	assert(args.maxLineSize == 2);
}

void shl_split_line_multiple_arg_test(void)
{
	struct shl_Line line = { .line = "ls -l\r", .length = 5 };
	struct shl_Lines args;

	args = shl_split_line(&line);
	
	assert(args.count == 2);
	assert(strcmp(args.lines[0], "ls") == 0);
	assert(strcmp(args.lines[1], "-l") == 0);
	assert(args.maxLineSize == 2);
}

void shl_split_line_comma_test(void)
{
	struct shl_Line line = { .line = "git commit -m \"commit name\"\r", .length = 27 };
	struct shl_Lines args;

	args = shl_split_line(&line);
	
	assert(args.count == 4);
	assert(strcmp(args.lines[0], "git") == 0);
	assert(strcmp(args.lines[1], "commit") == 0);
	assert(strcmp(args.lines[2], "-m") == 0);
	assert(strcmp(args.lines[3], "\"commit name\"") == 0);
	assert(args.maxLineSize == 13);
}

int main(void)
{
	shl_split_line_one_arg_test();
	shl_split_line_multiple_arg_test();

	return 0;
}
