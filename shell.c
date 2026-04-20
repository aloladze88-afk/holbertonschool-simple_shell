#include "shell.h"

/**
 * execute_command - Executes a command without arguments
 * @command: The command to execute (full path only)
 * @shell_name: Name of the shell program
 */

void execute_command(char *command, char *shell_name)
{
	pid_t pid;
	int status;
	char *args[2];

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (pid == 0)
	{
		args[0] = command;
		args[1] = NULL;
		execve(command, args, environ);
		fprintf(stderr, "%s: No such file or directory\n", shell_name);
		exit(EXIT_FAILURE);
	}
	else
	{
		wait(&status);
	}
}
