#include "main.h"

/**
 * match_builtin - check whether a line matches a built-in command exactly
 * @line: input line to inspect
 * @name: built-in command name
 *
 * Return: 1 if the line is exactly @name with optional surrounding spaces,
 * 0 otherwise
 */
static int match_builtin(const char *line, const char *name)
{
	size_t len;

	while (*line == ' ' || *line == '\t')
		line++;
	len = strlen(name);
	if (strncmp(line, name, len) != 0)
		return (0);
	line += len;
	while (*line == ' ' || *line == '\t')
		line++;
	return (*line == '\0');
}

/**
 * print_env_builtin - print the current environment
 *
 * Return: always 0
 */
static int print_env_builtin(void)
{
	int i;

	for (i = 0; environ[i] != NULL; i++)
	{
		write(STDOUT_FILENO, environ[i], strlen(environ[i]));
		write(STDOUT_FILENO, "\n", 1);
	}
	return (0);
}

/**
 * handle_builtin - execute supported built-ins
 * @line: input line entered by the user
 * @last_status: current shell status before the built-in runs
 * @handled: set to 1 if a built-in matched, 0 otherwise
 * @should_exit: set to 1 when the shell must exit, 0 otherwise
 *
 * Return: updated shell status
 */
int handle_builtin(char *line, int last_status, int *handled, int *should_exit)
{
	*handled = 1;
	*should_exit = 0;
	if (match_builtin(line, "exit"))
	{
		*should_exit = 1;
		return (last_status);
	}
	if (match_builtin(line, "env"))
		return (print_env_builtin());
	*handled = 0;
	return (last_status);
}
