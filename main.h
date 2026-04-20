#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

int execute_command(char *line, char *prog_name, int count);

#endif /* MAIN_H */
