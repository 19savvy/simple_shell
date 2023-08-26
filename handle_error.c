#include "shell.h"
#define BUFFER_SIZE 120
/**
 * handle_pid_error - handle errors relating
 * to process ID
 * @pid: the pid to handle
 */
void handle_pid_error(pid_t pid)
{
	if (pid == -1)
	{
		perror("Error!");
		exit(1);
	}
}

/**
 * is_delim - checks if a character is
 * a delimiter
 * @c: the character to be checked
 * @delim: A pointer to a null-terminated string of
 * delimiter characters
 * Return: 1 if it is a delimiter else return 0
 */

unsigned int is_delim(char c, char *delim)
{
	while (*delim != '\0')
	{
		if (c == *delim)
			return (1);
		delim++;
	}
	return (0);
}

/**
 * execute_shell_command - executes a shell command
 * @command: the command to be executed
 * @argv: an array of strings representing the command arguments
 * @env: an array od strings representing the environment variable
 */

void execute_shell_command(char *command, char **argv, char **env)
{
	pid_t pid;
    int status, i;

    if (command[0] == '#') {
        int print_previous = 0;
        for (i = 1; argv[i] != NULL; i++) {
            if (print_previous) {
                write(STDOUT_FILENO, argv[i], strlen(argv[i]));
                write(STDOUT_FILENO, " ", 1);
            }
            if (strcmp(argv[i], "#") == 0) {
                print_previous = 1;
            }
        }
        write(STDOUT_FILENO, "\n", 1);
        return;
    }


    pid = fork();
    if (pid == -1) {
        perror("Fork failed");
        exit(1);
    }

    if (pid == 0) {
        if (execute_(command, argv, env) == -1)
		{
			exit(127);
		}
        exit(0);
    }

    if (wait(&status) == -1) {
        exit(1);
    }
}
