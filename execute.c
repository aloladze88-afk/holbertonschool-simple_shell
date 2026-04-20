#include "main.h"

/**
 * execute_command - runs one command in a child process
 * @line: command to run, given as an absolute path
 * @prog_name: shell name for error messages
 */
void execute_command(char *line, char *prog_name)
{
	pid_t pid;
	int status;
	char *argv[2];

	argv[0] = line;
	argv[1] = NULL;

	pid = fork();
	if (pid == -1)
	{
		perror(prog_name);
		return;
	}

	if (pid == 0)
	{
		execve(line, argv, environ);
		perror(prog_name);
		free(line);
		exit(127);
	}

	wait(&status);
}