#include "main.h"

/**
 * execute_command - fork, execve, and wait for a command
 * @line: absolute path of the command to run
 * @prog_name: shell's argv[0], used as the error prefix
 * @count: current input line number, used in error messages
 *
 * Return: exit status of the child (126 on EACCES, 127 on ENOENT),
 * or 1 on fork failure
 */
int execute_command(char *line, char *prog_name, int count)
{
	pid_t pid;
	int status, exit_code;
	char *argv[2], *msg;

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
		if (errno == EACCES)
		{
			msg = "Permission denied";
			exit_code = 126;
		}
		else
		{
			msg = "not found";
			exit_code = 127;
		}
		fprintf(stderr, "%s: %d: %s: %s\n", prog_name, count, line, msg);
		free(line);
		exit(exit_code);
	}
	wait(&status);
	return (WEXITSTATUS(status));
}
