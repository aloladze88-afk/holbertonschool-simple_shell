#include "shell.h"

/**
 * main - entry point of the simple shell 0.1
 * @ac: argument count (unused)
 * @av: argument vector; av[0] is used as the error prefix
 *
 * Return: Always 0
 */
int main(int argc, char **argv)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	int interactive = isatty(STDIN_FILENO);

	(void)ac;
	line = NULL;
	len = 0;
	interactive = isatty(STDIN_FILENO);
	while (1)
	{
		if (interactive)
			write(STDOUT_FILENO, "($) ", 4);
		nread = getline(&line, &len, stdin);

		if (nread == -1)
		{
			if (interactive)
				printf("\n");
			break;
		}
		if (nread > 0 && line[nread - 1] == '\n')
			line[nread - 1] = '\0';
		if (line[0] == '\0')
			continue;
		execute_command(line, av[0]);
	}
	return (0);
}
