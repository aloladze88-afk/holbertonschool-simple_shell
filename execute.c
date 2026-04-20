#include "main.h"

/**
 * split_line - split an input line into arguments
 * @line: command line buffer to tokenize
 *
 * Return: newly allocated argv array, or NULL on failure
 */
static char **split_line(char *line)
{
	char **argv, **tmp;
	char *token;
	size_t size, count;

	size = 8;
	count = 0;
	argv = malloc(size * sizeof(char *));
	if (argv == NULL)
		return (NULL);

	token = strtok(line, " \t");
	while (token != NULL)
	{
		if (count + 1 >= size)
		{
			size *= 2;
			tmp = realloc(argv, size * sizeof(char *));
			if (tmp == NULL)
			{
				free(argv);
				return (NULL);
			}
			argv = tmp;
		}
		argv[count++] = token;
		token = strtok(NULL, " \t");
	}
	argv[count] = NULL;
	return (argv);
}

/**
 * execute_command - fork, execve, and wait for a command
 * @line: command line to run
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
	char **argv, *command, *msg;

	argv = split_line(line);
	if (argv == NULL)
	{
		perror(prog_name);
		return (1);
	}
	if (argv[0] == NULL)
	{
		free(argv);
		return (0);
	}
	command = argv[0];
	pid = fork();
	if (pid == -1)
	{
		free(argv);
		perror(prog_name);
		return (1);
	}
	if (pid == 0)
	{
		execve(command, argv, environ);
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
		fprintf(stderr, "%s: %d: %s: %s\n", prog_name, count, command, msg);
		free(argv);
		free(line);
		exit(exit_code);
	}
	waitpid(pid, &status, 0);
	free(argv);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
