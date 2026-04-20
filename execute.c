#include "main.h"

/**
 * execute_command - fork, execve, and wait for a command
 * @line: absolute path of the command to run
 * @prog_name: shell's argv[0], used as the error prefix
 * @count: current input line number, used in error messages
 *
 * Return: exit status of the child, or 1 on fork/wait failure
 */
int execute_command(char *line, char *prog_name, int count)
{
	pid_t pid;
	int status;
	int exit_code;
	char *argv[2];
	char *msg;

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

		dprintf(STDERR_FILENO, "%s: %d: %s: %s\n",
			prog_name, count, line, msg);
		free(line);
		_exit(exit_code);
	}

	if (waitpid(pid, &status, 0) == -1)
		return (1);

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));

	return (1);
}
