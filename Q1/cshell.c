#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>


void parseTokens(char * buffer);
int print(char * token);
int log_function();
int theme(char * token);
int exit_function();
int handle_env_vars(char * token);
int has_a_space(char * buffer);
int execute_command(char * buffer);


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
    while(1)
    {
        if(strcmp(ansiColor, "red") == 0)
        {
            printf("\e[0;31m$cshell ");
        }
        else if(strcmp(ansiColor, "green") == 0)
        {
            printf("\e[0;32m$cshell ");

        }
        else if(strcmp(ansiColor, "yellow") == 0)
        {
            printf("\e[0;33m$cshell ");

        }
        else {
            printf("$cshell ");

        }

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
        char copy_buffer[1000];
        strcpy(copy_buffer, buffer);
        char * new_token = strtok(copy_buffer, "\n");
        command_info.name = new_token;
        command_info.time = *timeinfo;

        token = strtok(buffer, "=");
        int exit_status = handle_env_vars(token);

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

        if(strcmp(token, "print") == 0) {
            command_info.name = "print";
            command_info.time = *timeinfo;
            int exit_status = print(token);
            command_info.code = exit_status;

            struct_array[i] = command_info;
            i++;
        }
        else if(strcmp(token, "log") == 0) {
            log_function();
        }
        else if(strcmp(token, "theme") == 0) {
            command_info.name = "theme";
            command_info.time = *timeinfo;
            int exit_status = theme(token);
            command_info.code = exit_status;

            struct_array[i] = command_info;
            i++;
        }
        else if(strcmp(token, "exit") == 0) {
            command_info.name = "exit";
            command_info.time = *timeinfo;
            int exit_status = exit_function();
            command_info.code = exit_status;

            struct_array[i] = command_info;
            i++;
        }

        else {
            execute_command(buffer);
            char copy[1000] = "";
            strcat(copy, buffer);
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
            }
        }
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
        strcpy(ansiColor, "red");
    }
    if(strcmp(token, "green") == 0)
    {
        strcpy(ansiColor, "green");
    }
    if(strcmp(token, "yellow") == 0)
    {
        strcpy(ansiColor, "yellow");
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
            printf("token: %s\n", token);
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