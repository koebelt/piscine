#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "my.h"

char* build_path(char* dirname, char* file)
{
    char* path;

    path = NULL;
    if(dirname)
    {
        path = malloc(my_strlen(dirname) + my_strlen(file) + 2);
        path = my_strcpy(path, dirname);
        path = my_strcat(path, "/");
        path = my_strcat(path, file);
    }
    return (path);
}

int my_exec(char** env_path, char** cmd, char** env)
{
    int i;
    char* filename;
    int permission;
    pid_t pid;

    permission = -1;
    filename = my_strdup(cmd[0]);
    i = 0;
    while(filename)
    {
        if(access(filename, X_OK) == 0)
        {
            pid = fork();
            if(pid == 0)
                execve(filename, cmd, env);
            else
                return (pid);
        }
        else if(errno == EACCES)
            permission = 0;
        free(filename);
        filename = build_path(env_path[i], cmd[0]);
        ++i;
    }
    return (permission);
}

char** find_path(char** env)
{
    int i;

    i = 0;
    while(env[i])
    {
        if(my_strncmp(env[i], "PATH=", 5) == 0)
            return (split_str(&env[i][5], ':'));
        ++i;
    }
    return (split_str("", ':'));
}

int main(int argc, char** argv, char** env)
{
    char buffer[4096];
    int len;
    char** env_path;
    char** cmd;
    int permission;
    int status;

    env_path = find_path(env);
    my_putstr("$>");
    while((len = read(STDIN_FILENO, buffer, 4096)) > 0)
    {
        cmd = my_str_to_wordtab(&buffer[1]);
        if(cmd[0] != 0)
        {
            permission = my_exec(env_path, cmd, env);
            if (permission == 0)
                my_putstr("Permission denied.\n");
            else if (permission == -1)
                my_putstr(my_strcat(buffer, " : command not found.\n"));
            else
                waitpid(permission, &status, 0);
        }
        my_putstr("$>");
    }
    return (0);
}