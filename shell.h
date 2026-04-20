#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

extern char **environ;

/* functions */
void display_prompt(void);
char *read_line(void);
void execute_command(char *line);

#endif
