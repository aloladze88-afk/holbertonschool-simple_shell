#include "main.h"

/**
 * execute_command - run a single command in a child process
 * @line: the command to run, given as an absolute path
 * @prog_name: the shell's own argv[0], used as the error prefix
 * @count: the number of the current input line, used in error messages
 *
 * Description: Forks a child which calls execve with @line as both
 * the program path and argv[0] of the invoked process, and passes
 * the current environ. The parent waits for the child to finish.
 * On execve failure, an error is printed to stderr in the format
 * "prog_name: count: line: not found" and the child frees the line
 * buffer before exiting with status 127, which mirrors standard
 * shell behaviour. The child's exit status is propagated back to
 * the caller via WEXITSTATUS so that the shell as a whole can
 * return the status of the last command on EOF.
 *
 * Return: the exit status of the child, or 1 on fork failure
 */
int execute_command(char *line, char *prog_name, int count)
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
		return (1);
	}
	if (pid == 0)
	{
		execve(line, argv, environ);
		fprintf(stderr, "%s: %d: %s: not found\n", prog_name, count, line);
		free(line);
		exit(127);
	}
	wait(&status);
	return (WEXITSTATUS(status));
}
