#include "main.h"

/**
 * print_exec_error - print shell-style execution error
 * @prog_name: shell argv[0]
 * @count: input line count
 * @command: command name as typed
 * @msg: error message
 */
static void print_exec_error(char *prog_name, int count, char *command, char *msg)
{
	fprintf(stderr, "%s: %d: %s: %s\n", prog_name, count, command, msg);
}

/**
 * join_path - build full path from directory and command
 * @dir: directory path
 * @command: command name
 *
 * Return: newly allocated full path or NULL on failure
 */
static char *join_path(const char *dir, const char *command)
{
	char *full;
	size_t len_dir, len_cmd;

	len_dir = strlen(dir);
	len_cmd = strlen(command);
	full = malloc(len_dir + len_cmd + 2);
	if (full == NULL)
		return (NULL);
	strcpy(full, dir);
	if (len_dir > 0 && dir[len_dir - 1] != '/')
		strcat(full, "/");
	strcat(full, command);
	return (full);
}

/**
 * resolve_command - resolve command using absolute path or PATH lookup
 * @command: command token from argv[0]
 * @status: output status code for failure cases
 * @msg: output message for failure cases
 *
 * Return: allocated executable path on success, NULL on failure
 */
static char *resolve_command(char *command, int *status, char **msg)
{
	char *path_env, *path_copy, *dir, *full;
	int saw_permission_denied;

	if (strchr(command, '/') != NULL)
	{
		if (access(command, F_OK) != 0)
		{
			*status = 127;
			*msg = "not found";
			return (NULL);
		}
		if (access(command, X_OK) != 0)
		{
			*status = 126;
			*msg = "Permission denied";
			return (NULL);
		}
		full = malloc(strlen(command) + 1);
		if (full == NULL)
			return (NULL);
		strcpy(full, command);
		return (full);
	}

	path_env = getenv("PATH");
	if (path_env == NULL || *path_env == '\0')
	{
		*status = 127;
		*msg = "not found";
		return (NULL);
	}

	path_copy = malloc(strlen(path_env) + 1);
	if (path_copy == NULL)
		return (NULL);
	strcpy(path_copy, path_env);
	saw_permission_denied = 0;

	dir = strtok(path_copy, ":");
	while (dir != NULL)
	{
		full = join_path(dir, command);
		if (full == NULL)
		{
			free(path_copy);
			return (NULL);
		}
		if (access(full, F_OK) == 0)
		{
			if (access(full, X_OK) == 0)
			{
				free(path_copy);
				return (full);
			}
			free(full);
			saw_permission_denied = 1;
			dir = strtok(NULL, ":");
			continue;
		}
		free(full);
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	if (saw_permission_denied)
	{
		*status = 126;
		*msg = "Permission denied";
		return (NULL);
	}
	*status = 127;
	*msg = "not found";
	return (NULL);
}

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
	resolved = resolve_command(command, &exit_code, &msg);
	if (resolved == NULL)
	{
		if (msg != NULL)
			print_exec_error(prog_name, count, command, msg);
		else
			perror(prog_name);
		free(argv);
		return (msg != NULL ? exit_code : 1);
	}
	pid = fork();
	if (pid == -1)
	{
		free(resolved);
		free(argv);
		perror(prog_name);
		return (1);
	}
	if (pid == 0)
	{
		execve(resolved, argv, environ);
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
		free(resolved);
		free(argv);
		free(line);
		exit(exit_code);
	}
	waitpid(pid, &status, 0);
	free(resolved);
	free(argv);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
