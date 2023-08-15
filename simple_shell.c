#include "shell.h"
/**
 * main - the main function for the simple shell program
 * @argc: number of arguments passed
 * @argv: the arguments passed
 * @env: the environment variable
 * Return: always 0
*/

int main(int argc, char **argv, char **env)
{
	size_t bufferSize = 0;
	ssize_t bytesRead;
	char *prompt = "$ ", *input = NULL, *token;

	while (true)
	{
		if (isatty(STDIN_FILENO) != 0)
			write(STDOUT_FILENO, prompt, strlen_(prompt));
		bytesRead = getline_(&input, &bufferSize, stdin);

		if (bytesRead == -1)
		{
			perror("Error!");
			free(input);
			exit(1);
		}
		if (input[bytesRead - 1] == '\n')
			input[bytesRead - 1] = '\0';
		char *argv[MAX_ARGS];
		int argc = 0;

		token = strtok_(input, " ;\n");
		while (token != NULL)
		{
			argv[argc++] = token;
			token = strtok_(NULL, " ;\n");
		}
		argv[argc] = NULL;
		if (argc > 0 && (strcmp(argv[0], "exit") == 0))
		{
			int exit_status = (argc > 1) ? atoi(argv[1]) : 0;

			free(input);
			exit_command(exit_status);
		}
		if (argc > 0)
			execute_shell_command(argv[0], argv, env);
	}
	free(input);
	return (0);
}


