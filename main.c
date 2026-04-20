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
 * main - entry point of the simple shell
 * @ac: argument count (unused)
 * @av: argument vector; av[0] is the error prefix
 *
 * Return: exit status of the last command executed
 */
int main(int ac, char **av)
{
	char *line;
	char *cmd;
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

		cmd = line;
		while (*cmd == ' ' || *cmd == '\t')
			cmd++;

		last_status = execute_command(cmd, av[0], count);
	}

	return (last_status);
}
