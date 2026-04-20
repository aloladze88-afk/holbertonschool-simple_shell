#include "main.h"

/**
 * execute_command - runs one command in a child process
 * @line: absolute path of the command
 * @prog_name: shell name, used in error messages
 *
 * Forks a child and runs the command with execve().
 * The parent waits for the child to finish.
 * If execve() fails, the child prints an error, frees line,
 * and exits with status 127.
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
