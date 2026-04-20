#include "main.h"

/**
 * execute_command - runs one command in a child process
 * @line: absolute path of the command
 * Forks a child and runs the command with execve().
 * The parent waits for the child to finish.
 * If execve() fails, the child prints an error and exits with status 127.
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
