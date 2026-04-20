#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

extern char **environ;

void display_prompt(void);
char *read_line(void);
void execute_command(char *line, char *prog_name);

#endif
