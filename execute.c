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
 * child_exec_failed - handle execve failure in child process
 * @resolved: resolved executable path
 * @argv: tokenized arguments
 * @line: original input line
 * @prog_name: shell argv[0]
 * @count: input line number
 * @command: command as typed
 */
static void child_exec_failed(char *resolved, char **argv, char *line,
	char *prog_name, int count, char *command)
{
	int exit_code;
	char *msg;

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
	print_exec_error(prog_name, count, command, msg);
	free(resolved);
	free(argv);
	free(line);
	_exit(exit_code);
}

/**
 * handle_resolve_failure - report command resolution failure
 * @argv: tokenized argument vector to free
 * @prog_name: shell argv[0]
 * @count: current input line number
 * @command: command as typed
 * @exit_code: status code prepared by resolve_command
 * @msg: error message from resolve_command, or NULL on internal failure
 *
 * Return: shell status code for the failure
 */
static int handle_resolve_failure(char **argv, char *prog_name, int count,
	char *command, int exit_code, char *msg)
{
	if (msg != NULL)
		print_exec_error(prog_name, count, command, msg);
	else
		perror(prog_name);
	free(argv);
	return (msg != NULL ? exit_code : 1);
}

/**
 * handle_fork_failure - report fork failure after freeing resources
 * @resolved: resolved executable path to free
 * @argv: tokenized argument vector to free
 * @prog_name: shell argv[0]
 *
 * Return: always 1
 */
static int handle_fork_failure(char *resolved, char **argv, char *prog_name)
{
	free(resolved);
	free(argv);
	perror(prog_name);
	return (1);
}

/**
 * wait_for_child - wait for one child and return its shell status
 * @pid: child process id
 *
 * Return: child's exit status, or 1 on wait failure
 */
static int wait_for_child(pid_t pid)
{
	int status;

	while (waitpid(pid, &status, 0) == -1)
	{
		if (errno != EINTR)
			return (1);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
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
	int exit_code;
	char **argv, *command, *msg, *resolved;

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
	msg = NULL;
	exit_code = 1;
	resolved = resolve_command(command, &exit_code, &msg);
	if (resolved == NULL)
		return (handle_resolve_failure(argv, prog_name, count, command,
			exit_code, msg));
	pid = fork();
	if (pid == -1)
		return (handle_fork_failure(resolved, argv, prog_name));
	if (pid == 0)
	{
		execve(resolved, argv, environ);
		child_exec_failed(resolved, argv, line, prog_name, count, command);
	}
	exit_code = wait_for_child(pid);
	free(resolved);
	free(argv);
	return (exit_code);
}
