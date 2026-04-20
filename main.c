#include "main.h"

/**
 * is_blank - test whether a string is empty or whitespace only
 * @s: string to inspect
 *
 * Return: 1 if @s has only spaces or tabs, 0 otherwise
 */
static int is_blank(const char *s)
{
	while (*s == ' ' || *s == '\t')
		s++;
	return (*s == '\0');
}

/**
 * is_exit_builtin - check whether the input is the exit built-in
 * @line: input line to inspect
 *
 * Return: 1 if the command is exit, 0 otherwise
 */
static int is_exit_builtin(const char *line)
{
	while (*line == ' ' || *line == '\t')
		line++;
	if (strncmp(line, "exit", 4) != 0)
		return (0);
	line += 4;
	while (*line == ' ' || *line == '\t')
		line++;
	return (*line == '\0');
}

/**
 * main - entry point of the simple shell
 * @ac: argument count (unused)
 * @av: argument vector; av[0] is the error prefix
 *
 * Return: exit status of the last command executed
 */
int main(int ac, char **av)
{
	char *line;
	size_t len;
	ssize_t nread;
	int interactive, count, last_status;

	(void)ac;
	line = NULL;
	len = 0;
	count = 0;
	last_status = 0;
	interactive = isatty(STDIN_FILENO);
	while (1)
	{
		if (interactive)
			write(STDOUT_FILENO, "($) ", 4);
		nread = getline(&line, &len, stdin);
		if (nread == -1)
		{
			free(line);
			if (interactive)
				write(STDOUT_FILENO, "\n", 1);
			return (last_status);
		}
		count++;
		if (nread > 0 && line[nread - 1] == '\n')
			line[nread - 1] = '\0';
		if (is_blank(line))
			continue;
		if (is_exit_builtin(line))
		{
			free(line);
			return (last_status);
		}
		last_status = execute_command(line, av[0], count);
	}
	return (last_status);
}
