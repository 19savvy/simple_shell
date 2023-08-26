#include "shell.h"
#define BUFFER_SIZE 1024
#define MAX_NUM_ARGUMENTS 100

int check_status(char *path, struct stat *buf)
{
	int value;

	value = stat(path, buf);
	if (value == 0)
		return (0);
	else
		return (1);
}

ssize_t getline_(char **lineptr, size_t *n, FILE *stream)
{

	static char buffer[BUFFER_SIZE];
	size_t line_length = 0;
	char c, *new_lineptr;
	int end_of_file = 0, buffer_size = 0, buffer_pos = 0;

	if (*n == 0 || *lineptr == NULL)
	{
		*n = BUFFER_SIZE;
		*lineptr = malloc(*n);
		if (!*lineptr)
			return (-1);
	}
	while (1)
	{
		if (buffer_pos >= buffer_size)
		{
			buffer_size = read(fileno(stream), buffer, BUFFER_SIZE);
			if (buffer_size == 0)
			{
				end_of_file = 1;
				break;
			}
			buffer_pos = 0;
		}
		if (buffer_size == -1)
			return (-1);
		c = buffer[buffer_pos++];

		if (line_length + 1 >= *n)
		{
			*n *= 2;
			new_lineptr = realloc(*lineptr, *n);

			if (!new_lineptr)
				return (-1);
			*lineptr = new_lineptr;
		}
		(*lineptr)[line_length++] = c;

		if (c == '\n')
			break;
	}
	if (line_length > 0)
	{
		(*lineptr)[line_length] = '\0';
		return (line_length);
	}
	else if (end_of_file)
		return (-1);
	else
		return (0);
}

char *find_command(const char *command)
{
    char *path = _getenv("PATH"), *path_copy, *dir, *full_path;
	struct stat buf;
    if (path == NULL) {
        return NULL;
    }

    path_copy = strdup(path);
    if (path_copy == NULL) {
        perror("Memory allocation error");
        exit(1);
    }

    dir = strtok(path_copy, ":");
    full_path = NULL;

    while (dir != NULL)
	{
        size_t full_path_length = strlen(dir) + strlen(command) + 2;
        full_path = (char *)malloc(full_path_length);

        if (full_path == NULL) {
            perror("Memory allocation error");
            exit(1);
        }

        snprintf(full_path, full_path_length, "%s/%s", dir, command);

        if (check_status(full_path, &buf) == 0)
		{
            free(path_copy);
            return full_path;
        }

        free(full_path);
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return (NULL);
}


int execute_(char *command, char **argv, char **env)
{
	if (strchr(command, '/') != NULL)
	{
		if (execve(command, argv, env) == -1)
		{
			return(-1);
		}
	}
	else
	{
		char *full_command = find_command(command);

		if (full_command == NULL)
		{
			write(STDERR_FILENO, "./hsh: 1: ", 10);
            write(STDERR_FILENO, argv[0], strlen(argv[0]));
            write(STDERR_FILENO, ": not found\n", 12);
			exit(127);
		}

		if (execve(full_command, argv, env) == -1)
		{
			return (-1);
		}
	}

	return (0);
}