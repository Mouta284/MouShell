#include "builtins.h"

char* builtin_str[] = {"cd", "exit"};
int (*builtin_func[])(char**) = {&cd, &sh_exit};

int builtin_num(){
    return sizeof(builtin_str)/sizeof(char*);
}

int cd(char** args){
    if(args[1] == NULL){
        fprintf(stderr, "sh-error: there is no argument for the cd command.");
        exit(EXIT_FAILURE);
    }
    else if(chdir(args[1]) != 0){
        perror("sh-error: ");
    }

    return 1;
}

int sh_exit(char** args){
    exit(EXIT_SUCCESS);
}
