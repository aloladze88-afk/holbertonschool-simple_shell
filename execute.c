#include "shell.h"

/**
 * execute_command - executes a command
 * @line: command line
 */
void execute_command(char *line)
{
    pid_t pid;
    char *argv[2];

    if (line[0] == '\0')
        return;

    argv[0] = line;
    argv[1] = NULL;

    pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    {
        if (execve(line, argv, environ) == -1)
        {
            perror("./shell");
            exit(1);
        }
    }
    else
    {
        wait(NULL);
    }
}
