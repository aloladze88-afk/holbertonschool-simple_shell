#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

int execute_command(char *line, char *prog_name, int count);
char *resolve_command(char *command, int *status, char **msg);
void print_exec_error(char *prog_name, int count, char *command, char *msg);

#endif /* MAIN_H */
