#include "shell.h"

/**
 * read_line - reads user input
 * Return: line or NULL on EOF
 */
char *read_line(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    nread = getline(&line, &len, stdin);

    if (nread == -1)
    {
        free(line);
        if (isatty(STDIN_FILENO))
            printf("\n");
        return (NULL);
    }

    /* remove newline */
    line[strcspn(line, "\n")] = '\0';

    return (line);
}
