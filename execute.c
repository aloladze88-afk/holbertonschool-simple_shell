#include "main.h"

/**
 * execute_command - run a single command in a child process
 * @line: the command to run, given as an absolute path
 * @prog_name: the shell's own argv[0], used as the error prefix
 *
 * Description: Forks a child which calls execve with @line as both
 * the program path and argv[0] of the invoked process, and passes
 * the current environ. The parent waits for the child to finish.
 * On execve failure, an error is printed with perror and the child
 * frees the line buffer before exiting with status 127, which
 * mirrors standard shell behaviour.
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
