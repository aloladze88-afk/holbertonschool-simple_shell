#include "main.h"

/**
 * read_line - reads one line from standard input
 *
 * Return: the line read, or NULL on EOF
 */

char read_line(void)
{
    charline = NULL;
    size_t len = 0;
    ssize_t read;

    read = getline(&line, &len, stdin);

    if (read == -1)
    {
        free(line);
        printf("\n");
        return (NULL);
    }

    line[strcspn(line, "\n")] = '\0';

    return (line);
}
