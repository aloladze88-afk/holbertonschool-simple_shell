#include "shell.h"

/**
 * display_prompt - prints shell prompt
 */
void display_prompt(void)
{
    if (isatty(STDIN_FILENO))
    {
        printf("#cisfun$ ");
        fflush(stdout);
    }
}
