#include "main.h"

/**
 * is_blank - check whether a string is empty or whitespace only
 * @s: the string to inspect
 *
 * Return: 1 if @s contains only spaces and tabs (or nothing), 0 otherwise
 */
static int is_blank(const char *s)
{
	while (*s == ' ' || *s == '\t')
		s++;
	return (*s == '\0');
}

/**
 * main - entry point of the simple shell 0.1
 * @ac: argument count (unused)
 * @av: argument vector; av[0] is used as the error prefix
 *
 * Description: Displays a prompt, reads one line from standard input,
 * strips the trailing newline, and passes the command to
 * execute_command. Blank and whitespace-only lines are ignored, but
 * are still counted for error-message line numbering, matching the
 * behaviour of /bin/sh. The shell exits cleanly when end-of-file is
 * reached (Ctrl+D in interactive mode), returning the exit status of
 * the last command executed.
 *
 * Return: the exit status of the last command executed, or 0 if none
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
		last_status = execute_command(line, av[0], count);
	}
	return (last_status);
}
