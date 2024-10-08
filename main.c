#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#include "shl_cli.h"
#include "shl_string.h"
#include "shl_output.h"

#define shl_DEBUG false
#if shl_DEBUG
	#include "shl_debug.h"
#endif

static char** history;
static uint_fast32_t history_position = 0;
static const uint_fast32_t max_history_position = 50;

_Bool shl_is_exit_command(struct shl_Args args)
{
	if (shl_string_compare(args.lines[0], "q", args.maxLineSize) == 0)
		return true;
	if (shl_string_compare(args.lines[0], "exit", args.maxLineSize) == 0)
		return true;
	if (shl_string_compare(args.lines[0], "quit", args.maxLineSize) == 0)
		return true;

	return false;
}

_Bool shl_is_echo_command(struct shl_Args args)
{
	if (shl_string_compare(args.lines[0], "echo", args.maxLineSize) == 0)
		return true;
	
	return false;
}

uint_fast32_t shl_echo_command(struct shl_Args args)
{
	for (uint_fast32_t i = 1; i < args.count; i++)
		printf("%s ", args.lines[i]);

	if (args.count > 0)
		printf("\n");

	return 1;
}

_Bool shl_is_help_command(struct shl_Args args)
{
	if (shl_string_compare(args.lines[0], "help", args.maxLineSize) == 0)
		return true;
	
	return false;
}

uint_fast32_t shl_help_command(void)
{
	printf("conch-shell by Alex Eski: help\n");

	printf("To exit, type q, exit, or quit.\n");

	return 1;
}

_Bool shl_is_cd_command(struct shl_Args args)
{
	if (shl_string_compare(args.lines[0], "cd", args.maxLineSize) == 0)
		return true;
	
	return false;
}

uint_fast32_t shl_cd_command(struct shl_Args args)
{
	if (args.lines[1] == NULL)
	{
		fprintf(stderr, "shl: no arguments to cd.\n");
		return 1;
	}

	if (chdir(args.lines[1]) != 0)
		fprintf(stderr, "shl: could not change directory.\n");

	return 1;
}

_Bool shl_is_history_command(struct shl_Args args)
{
	if (shl_string_compare(args.lines[0], "history", args.maxLineSize) == 0)
		return true;
	
	return false;
}

uint_fast32_t shl_history_command()
{
	for (uint_fast32_t i = 0; i < history_position; i++) {
		printf("%s\n", history[i]);
	}
	return 1;
}

uint_fast32_t shl_launch_process(struct shl_Args args)
{
	#if shl_DEBUG
		shl_debug_launch_process(args);
	#endif /* ifdef shl_DEBUG */

	pid_t pid;
	int status;

	pid = fork();
	if (pid == 0)
	{
		int exec_result = execvp(args.lines[0], args.lines);
		if (exec_result == -1)
			perror(RED "conch-shell: error when creating child process" RESET);
		return 0;
	}
	else if (pid < 0)
	{
		perror(RED "conch-shell: error when forking process" RESET);
	}
	else
	{
		do {
			waitpid(pid, &status, WUNTRACED);
		}while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 1;
}

uint_fast32_t shl_execute_command(struct shl_Args args)
{
	if (shl_is_exit_command(args))
		return 0;

	if (shl_is_echo_command(args))
		return shl_echo_command(args);

	if (shl_is_help_command(args))
		return shl_help_command();

	if (shl_is_cd_command(args))
		return shl_cd_command(args);

	if (shl_is_history_command(args))
		return shl_history_command();

	return shl_launch_process(args);
}

int main (void)
{
	uint_fast32_t status = 1;
	struct shl_Line line;
	struct shl_Args args;
	struct shl_Directory directory;
	char* getcwd_result = NULL;
	char* user = getenv("USER");
	history = malloc(sizeof(char*) * max_history_position);

	do {
		getcwd_result = getcwd(directory.path, sizeof(directory.path));
		if (getcwd_result == NULL)
		{
			perror(RED "conch-shell: error when getting current directory" RESET);
			status = 0;
		}

		printf(GREEN "%s" WHITE ":" CYAN "%s" RESET "$ " , user, directory.path);

		line = shl_line_read();
		if (!shl_line_is_valid(line))
		{
			free(line.line);
			continue;
		}
		history[history_position] = malloc(sizeof(char) * line.length);
		shl_string_copy(history[history_position++], line.line, line.length);
		#if shl_DEBUG
			shl_debug_line(line);
		#endif
		
		args = shl_line_split(line);
		if (!shl_args_is_valid(args))
		{
			printf(RED "Invalid arguments.\n" RESET);
			free(line.line);
			for (uint_fast32_t i = 0; i < args.count; i++)
				free(args.lines[i]);
			free(args.lines);
			continue;
		}

		status = shl_execute_command(args);

		free(line.line);
		for (uint_fast32_t i = 0; i < args.count; i++)
			free(args.lines[i]);
		free(args.lines);
	} while (status);

	return EXIT_SUCCESS;
}
