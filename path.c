#include "main.h"

/**
 * print_exec_error - print shell-style execution error
 * @prog_name: shell argv[0]
 * @count: input line count
 * @command: command name as typed
 * @msg: error message
 */
void print_exec_error(char *prog_name, int count, char *command, char *msg)
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
 * check_direct_command - validate command containing '/'
 * @command: command name as typed
 * @status: output status code on failure
 * @msg: output error message on failure
 *
 * Return: duplicated command path or NULL
 */
static char *check_direct_command(char *command, int *status, char **msg)
{
	char *full;

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

/**
 * search_path_dirs - search executable in PATH directories
 * @path_copy: writable PATH copy
 * @command: command name as typed
 * @status: output status code on failure
 * @msg: output error message on failure
 *
 * Return: allocated executable path or NULL
 */
static char *search_path_dirs(char *path_copy, char *command, int *status,
	char **msg)
{
	char *dir, *next, *full;
	char saved;
	struct stat st;
	int denied;

	denied = 0;
	dir = path_copy;
	while (dir != NULL)
	{
		next = dir;
		while (*next != ':' && *next != '\0')
			next++;
		saved = *next;
		*next = '\0';
		full = join_path(dir, command);
		*next = saved;
		if (full == NULL)
			return (NULL);
		if (access(full, F_OK) == 0)
		{
			if (stat(full, &st) == 0 && !S_ISDIR(st.st_mode) &&
				access(full, X_OK) == 0)
				return (full);
			denied = 1;
		}
		free(full);
		dir = (saved == '\0') ? NULL : next + 1;
	}
	*status = denied ? 126 : 127;
	*msg = denied ? "Permission denied" : "not found";
	return (NULL);
}

/**
 * resolve_command - resolve command using absolute path or PATH lookup
 * @command: command token from argv[0]
 * @status: output status code for failure cases
 * @msg: output message for failure cases
 *
 * Return: allocated executable path on success, NULL on failure
 */
char *resolve_command(char *command, int *status, char **msg)
{
	char *path_env, *path_copy, *full;

	if (strchr(command, '/') != NULL)
		return (check_direct_command(command, status, msg));
	path_env = getenv("PATH");
	if (path_env == NULL)
	{
		*status = 127;
		*msg = "not found";
		return (NULL);
	}
	path_copy = malloc(strlen(path_env) + 1);
	if (path_copy == NULL)
		return (NULL);
	strcpy(path_copy, path_env);
	full = search_path_dirs(path_copy, command, status, msg);
	free(path_copy);
	return (full);
}
