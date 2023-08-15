#ifndef _SHELL_H_
#define _SHELL_H_

#define MAX_ARGS 100
#define MAX_PATH_LENGTH 100
extern char **environ;

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
char **str_tok(char *str, char *delim, int *count);
int check_status(char *path, struct stat *buf);
int execute_(char *command, char **argv, char **env);
void handle_pid_error(pid_t pid);
char *find_command(const char *command, char **env);
char *strdup_(const char *str);
char *strchr_(char *str, char c);
void exit_command(int status);
void execute_shell_command(char *command, char **argv, char **env);
ssize_t getline_(char **lineptr, size_t *n, FILE *stream);
size_t tokens_count(char *tokens);
char *strtok_(char *srcString, char *delim);
size_t strlen_(const char* str);
unsigned int is_delim(char c, char *delim);
/* ENV */
char *_getenv(const char *name);
void print_path_directories(void);
int _setenv(const char *name, const char *value, int overwrite);

typedef struct PathNode {
    char *directory;
    struct PathNode *next;
} PathNode;
typedef struct builtin {
    char *type;
    int (*func)();
} builtin_commands;

#endif /* _SHELL_H_ */

