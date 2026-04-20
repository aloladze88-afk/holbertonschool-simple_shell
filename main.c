#include "main.h"

/**
 * main - entry point of the simple shell 0.1
 * @ac: argument count
 * @av: argument vector; av[0] is used as the error prefix
 *
 * Description: Displays a prompt, reads one line from standard input,
 * strips the trailing newline, and passes the command to
 * execute_command. Empty lines are ignored. The shell exits cleanly
 * when end-of-file is reached (Ctrl+D in interactive mode).
 * Return: 0 on clean exit
 */

int main(int ac, char **av)
{
	char *line;
	size_t len;
	ssize_t nread;
	int interactive;

	(void)ac;
	line = NULL;
	len = 0;
	interactive = isatty(STDIN_FILENO);
	while (1)
	{
		if (interactive)
        {
			write(STDOUT_FILENO, "($) ", 4);
		nread = getline(&line, &len, stdin);
        }
		if (nread == -1)
		{
			free(line);
			if (interactive)
            {
				write(STDOUT_FILENO, "\n", 1);
            }
			return (0);
		}
		if (nread > 0 && line[nread - 1] == '\n')
        {
			line[nread - 1] = '\0';
        }
		if (line[0] == '\0')
        {
			continue;
		execute_command(line, av[0]);
        }
	}
	return (0);
}
