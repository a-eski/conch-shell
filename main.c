#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "shl_debug.h"
#include "shl_types.h"
#include "shl_cli.h"
#include "shl_string.h"

_Bool shl_is_exit_command(struct shl_Lines* args)
{
	if (shl_string_compare(args->lines[0], "q", args->maxLineSize) == 0)
		return true;
	if (shl_string_compare(args->lines[0], "exit", args->maxLineSize) == 0)
		return true;
	if (shl_string_compare(args->lines[0], "quit", args->maxLineSize) == 0)
		return true;

	return false;
}

_Bool shl_is_echo_command(struct shl_Lines* args)
{
	if (shl_string_compare(args->lines[0], "echo", args->maxLineSize) == 0)
		return true;
	
	return false;
}

int shl_echo_command(struct shl_Lines* args)
{
	for (uint_fast32_t i = 1; i < args->count; i++)
		printf("%s\n", args->lines[i]);

	return 1;
}

uint_fast32_t shl_launch_process(struct shl_Lines* args)
{
	shl_debug_args(args);
	// pid_t pid;
	// int status;
	//
	// pid = fork();
	// if (pid == 0)
	// {
	// 	if (execvp(args.lines[0], args.lines))
	// 		perror("shl");
	// 	exit(EXIT_FAILURE);
	// }
	// else if (pid < 0)
	// {
	// 	perror("shl");
	// }
	// else
	// {
	// 	do {
	// 		waitpid(pid, &status, WUNTRACED);
	// 	}while (!WIFEXITED(status) && !WIFSIGNALED(status));
	// }

	return 1;
}

_Bool shl_is_help_command(struct shl_Lines* args)
{
	if (shl_string_compare(args->lines[0], "help", args->maxLineSize) == 0)
		return true;
	
	return false;
}

int shl_execute_command(struct shl_Lines* args)
{
	if (shl_is_exit_command(args))
		return 0;

	if (shl_is_echo_command(args))
		return shl_echo_command(args);

	if (shl_is_help_command(args))
		return 0;

	return shl_launch_process(args);
}

int main (void)
{
	uint_fast32_t status = 1;
	struct shl_Line line;
	struct shl_Lines args;

	do {
		printf("> ");
		line = shl_read_line();
		if (!shl_is_valid_line(&line))
		{
			continue;
		}
		// shl_debug_line(line);

		args = shl_split_line(&line);
		if (!shl_is_valid_args(&args))
		{
			printf("Invalid arguments.\n");
			continue;
		}
		// shl_debug_args(args);

		status = shl_execute_command(&args);

		free(line.line);
		for (uint_fast32_t i = 0; i < args.count; i++)
			free(args.lines[i]);
		free(args.lines);
	} while (status);

	return EXIT_SUCCESS;
}
