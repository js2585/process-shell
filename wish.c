#include "wish.h"

char* PATH[1024];
int path_length = 0;

char* trim(char* s)
{
    while (isspace(*s)) s++;
    char* back = s + strlen(s);
    while (isspace(*--back));
    *(back + 1) = '\0';
    return s;
}

void parse_line(char* line, char** args)
{
    line = trim(line);
    args[0] = strsep(&line, " ");
    args[1] = line;
}

int main(int argc, char* argv[])
{
    PATH[0] = "/bin/";
    path_length = 1;
    while (1)
    {
        printf("wish> ");
        int buffer_size = 1024;
        size_t linecap = buffer_size * sizeof(char);
        char* line = (char*)malloc(linecap);
        getline(&line, &linecap, stdin);
        char* args[2] = { NULL, NULL };
        parse_line(line, args);
        if (strcmp(args[0], "exit") == 0)
        {
            exit(0);
        }
        else if (strcmp(args[0], "cd") == 0)
        {
            if (args[1] == NULL)
            {
                printf("expected argument to \"cd\"\n");
            }
            else
            {
                chdir(args[1]);
            }
        }
        else if (strcmp(args[0], "path") == 0)
        {
            path_length = 0;
            char* token;
            while ((token = strsep(&args[1], " ")) != NULL)
            {
                PATH[path_length] = token;
                path_length++;
                if (path_length >= 1024)
                {
                    break;
                }
            }
        }
        else
        {
            pid_t pid = fork();
            if (pid == 0)
            {
                for (int i = 0; i < path_length; i++)
                {
                    char path[strlen(PATH[i]) + strlen(args[0]) + 1];
                    strcpy(path, PATH[i]);
                    strcat(path, args[0]);
                    int arg_length = strlen(args[0]);
                    if (args[1] != NULL)
                    {
                        arg_length += strlen(args[1]);
                    }
                    char new_args[arg_length + 1];
                    strcpy(new_args, args[0]);
                    if (args[1] != NULL)
                    {
                        strcat(new_args, " ");
                        strcat(new_args, args[1]);
                    }
                    if (access(path, X_OK) == 0)
                    {
                        execl(path, new_args);
                        exit(0);
                    }
                }
                execl(args[0], args[1]);
                exit(0);
            }
            else
            {
                wait(NULL);
            }
        }
        free(line);
    }
}