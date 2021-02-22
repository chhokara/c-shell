#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <sys/wait.h>


void parseTokens(char * buffer);
int print(char * token);
int log_function();
int theme(char * token);
int exit_function();
int handle_env_vars(char * token);
int has_a_space(char * buffer);
int exec_command(char * buffer);
int script_mode(char * file);


struct Command {
    char * name;
    struct tm time;
    int code;
}; 

struct Command struct_array[1000];

struct EnvVar {
    char name[1000];
    char value[1000];
};

struct EnvVar env_array[1000];

int i = 0;
int j = 0;

char ansiColor[1000];

int main (int argc, char * argv[])
{
    char buffer[1000];
    int k;
    for(k = 0; k < 1000; k++)
    {
        buffer[k] = '0';
    }
    if(argv[1])
    {
        script_mode(argv[1]);
        return 0;
    }
    while(1)
    {
        printf("$cshell ");

        if(fgets(buffer, 1000, stdin)) 
        {
            parseTokens(buffer);
        }
    }
    return 0;
}

void parseTokens(char * buffer)
{
    int space_flag = has_a_space(buffer);
    struct Command command_info; 
    time_t t;
    struct tm * timeinfo;
    time(&t);
    timeinfo = localtime(&t);


    char * token = "";
    if(buffer[0] == '$')
    {   
        char * copy = strdup(buffer);
        copy = strtok(copy, "\n");
        token = strtok(buffer, "=");
        int exit_status = handle_env_vars(token);
        command_info.name = copy;
        command_info.time = *timeinfo;
        command_info.code = exit_status;
        struct_array[i] = command_info;
        i++;
    }
    else 
    {
        if(space_flag)
        {
            token = strtok(buffer, " ");
        }
        else 
        {
            token = strtok(buffer, "\n");
        }

        if(strcmp(token, "print") == 0 || strcmp(token, "print\n") == 0) {
            command_info.name = "print";
            command_info.time = *timeinfo;
            int exit_status = print(token);
            command_info.code = exit_status;

            struct_array[i] = command_info;
            i++;
        }
        else if(strcmp(token, "log") == 0 || strcmp(token, "log\n") == 0) {
            command_info.name = "log";
            command_info.time = *timeinfo;
            int exit_status = log_function();
            command_info.code = exit_status;

            struct_array[i] = command_info;
            i++;
        }
        else if(strcmp(token, "theme") == 0 || strcmp(token, "theme\n") == 0) {
            command_info.name = "theme";
            command_info.time = *timeinfo;
            int exit_status = theme(token);
            command_info.code = exit_status;

            struct_array[i] = command_info;
            i++;
        }
        else if(strcmp(token, "exit") == 0 || strcmp(token, "exit\n") == 0) {
            command_info.name = "exit";
            command_info.time = *timeinfo;
            int exit_status = exit_function();
            command_info.code = exit_status;

            struct_array[i] = command_info;
            i++;
        }

        else {
            char * copy = strdup(token);
            exec_command(token);
            copy = strtok(copy, "\n");
            command_info.name = copy;
            command_info.time = *timeinfo;
            command_info.code = 0;

            struct_array[i] = command_info;
            i++;
        }
    }

    
    
}

int print(char * token)
{
    token = strtok(NULL, "\n");
    if(token[0] == '$')
    {   
        int i = 0;
        for(i = 0; i < 1000; i++)
        {
            if(strcmp(env_array[i].name, token) == 0)
            {
                printf("%s\n", env_array[i].value);
                return 0;
            }
            
        }
        printf("variable not found\n");

    }
    else 
    {
        while(token != NULL)
        {
            printf("%s\n", token);
            token = strtok(NULL, " ");
        }
    }
    
    return 0;
}

int log_function()
{
    int i = 0;
    for(i = 0; i < 1000; i++)
    {
        if(struct_array[i].name || struct_array[i].code)
        {
            printf("%s %s %d\n", asctime(&struct_array[i].time), struct_array[i].name, struct_array[i].code);
        }
    }
    return 0;
}

int theme(char * token)
{
    token = strtok(NULL, "\n");
    if(strcmp(token, "red") == 0)
    {
        printf("\e[0;31m");
    }
    if(strcmp(token, "green") == 0)
    {
        printf("\e[0;32m");
    }
    if(strcmp(token, "blue") == 0)
    {
        printf("\e[0;34m");
    }
    return 0;
}

int exit_function()
{
    exit(0);
}

int handle_env_vars(char * token)
{
    int i = 0;
    for(i = 0; i < 1000; i++)
    {
        if(strcmp(env_array[i].name,token) == 0)
        {
            token = strtok(NULL, "\n");
            strcpy(env_array[i].value, token);
            return 0;
        }
        
    }
    struct EnvVar environment_variables;
    strcpy(environment_variables.name, token);
    token = strtok(NULL, "\n");
    strcpy(environment_variables.value, token);
    env_array[j] = environment_variables;
    j++;


    return 0;
}

int has_a_space(char * buffer)
{
    int i = 0;
    for(i = 0; i < 1000; i++)
    {
        if(buffer[i] == ' ')
        {
            return 1;
        }

    }
    return 0;
}

int exec_command(char * buffer)
{
    int i = 0;
    int j = 0;
    char * tokens[1000];    

    while(buffer != NULL)
    {
        tokens[i++] = buffer;
        buffer = strtok(NULL, " ");
    }
    tokens[i-1] = strtok(tokens[i-1], "\n");

    char * args[i+1];
    args[i] = NULL;
    for(j = 0; j < i; j++)
    {
        args[j] = tokens[j];
    }
    
    int fd[2];
    char buf[1];
    char errorMsg[100] = "Missing keyword or command, or permission problem\n";
    if(pipe(fd) == -1)
    {
        perror("error creating pipe");
        return -1;
    }

    pid_t fc; 
    fc = fork();
    if(fc < 0)
    {
        printf("fork failed\n");
        exit(1);
    } 
    else if(fc == 0) 
    {
        // child process
        
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        dup2(fd[1], STDERR_FILENO);
        close(fd[1]);
        if(execvp(args[0], args) == -1)
        {
            write(STDERR_FILENO, errorMsg, sizeof(errorMsg));
            _exit(0);
        }    
    }
    else 
    {
        // parent process
        close(fd[1]);
        while(read(fd[0], buf, sizeof(buf)) > 0)
        {
            write(STDOUT_FILENO, buf, sizeof(buf));
        }
        close(fd[0]);
        wait(NULL);

    }
    return 0;
}

int script_mode(char * file)
{
    FILE* OS_file;
    char buf[1000];
    char * token;
    OS_file = fopen(file, "r");

    while(!feof(OS_file))
    {
        fgets(buf, 1000, OS_file);
        token = strtok(buf, "\n");
        parseTokens(token);
    }
    fclose(OS_file);

    return 0;
}