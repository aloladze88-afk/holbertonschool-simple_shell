#include "main.h"

/**
 * main - entry point of the simple shell
 * @ac: argument count
 * @av: argument vector
 *
 * Return: 0 on success
 */
int main(int ac, char **av)
{
	char *line;
	size_t len;
	ssize_t nread;
	int interactive;
	int count;

	(void)ac;
	line = NULL;
	len = 0;
	count = 0;
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
			return (0);
		}

		count++;

		if (nread > 0 && line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		if (line[0] == '\0')
			continue;

		execute_command(line, av[0], count);
	}

	return (0);
}
