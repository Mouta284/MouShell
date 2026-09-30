#include "builtins.h"

#define READ_LINE_BUFFER 1024
#define ARGS_BUFFER 64
#define SEP_TOKENS " \n\r\t\a"

char* read_line(){
    int buffsize = READ_LINE_BUFFER;
    int position = 0;   
    char* buffer = malloc(sizeof(char)*buffsize);
    int c;

    if(!buffer) {
        fprintf(stderr, "sh-error: allocation error related to the buffer");
        exit(EXIT_FAILURE);
    }

    while(1){
        c = getchar();
        // Verifying if we are at the end of the command
        if(c == EOF || c == '\n'){
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        position++;

        // Checking if the user is writting too much commands or text into the buffer
        // So we reallocate the buffer size
        if(position >= buffsize){
            buffsize += READ_LINE_BUFFER;
            buffer = realloc(buffer, buffsize);
            
            // Checking for buffer reallocation errors
            if(!buffer){
                fprintf(stderr, "sh-error: the buffer size reallocation went wrong");
                exit(EXIT_FAILURE);
            }
        }
    }
}

char** split_line(char* line){
    int buffsize = ARGS_BUFFER;
    int position = 0;
    char** buffer = malloc(sizeof(char*)*buffsize);
    char* token;

    if(!buffer){
        fprintf(stderr, "sh-error: the buffer was not correctly allocated!");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, SEP_TOKENS);
    while (token != NULL)
    {
        buffer[position] = token;
        position++;

        if(position >= buffsize){
            buffsize += ARGS_BUFFER;
            buffer = realloc(buffer, buffsize);

            if(!buffer){
                fprintf(stderr, "sh-error: the buffer size reallocation went wrong");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, SEP_TOKENS);
    }

    // Here we just add the NULL value like we do with '\0' value 
    // but it is for tokens
    buffer[position] = NULL;
    return buffer;
}

int exec_program(char** args){
    pid_t pid, wait_pid;
    int status;

    // Forking the current process
    pid = fork();

    // if the process running is the child of the original
    if(pid == 0){
        if(execvp(args[0], args) == -1){
            perror("sh-error: ");
        }
        exit(EXIT_SUCCESS);
    }
    else if(pid == -1){
        perror("sh-error: ");
    }
    else {
        do {
            wait_pid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int sh_execute(char** args){

    if(args[0] == NULL){
        fprintf(stderr, "sh-error: there is no command written.");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < builtin_num(); i++){
        if(strcmp(builtin_str[i], args[0]) == 0){
            return (*builtin_func[i])(args);
        }
    }
    
    return exec_program(args);
}

void sh_loop(){
    char* line;
    char** args;
    int status;

    do {
        printf("moushell > ");

        line = read_line();
        args = split_line(line);
        status = sh_execute(args);

        free(line);
        free(args);
    } while(status);
}

int main(int argc, char** argv){
    // Init Shell
    printf("Welcome to the MouShell v.0.1 \n");

    // Shell loop
    sh_loop();

    return EXIT_SUCCESS;
}