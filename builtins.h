#ifndef BUILTINS_H
#define BUILTINS_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int cd(char** args);
int sh_exit(char** args);
int builtin_num();
// int help(char** args);

extern char* builtin_str[];
extern int (*builtin_func[])(char**);

#endif
