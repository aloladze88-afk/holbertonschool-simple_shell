#include "shell.h"

/**
 * main - simple shell loop
 * Return: Always 0
 */
int main(void)
{
    char *line;

    while (1)
    {
        display_prompt();

        line = read_line();
        if (line == NULL)
            break;

        execute_command(line);
        free(line);
    }

    return (0);
}
