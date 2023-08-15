#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

ssize_t getline_(char **lineptr, size_t *n, FILE *stream)
{
    static char buffer[BUFFER_SIZE];
    static size_t buffer_pos = 0;
    static ssize_t buffer_size = 0;
	ssize_t line_length = 0;
    int end_of_file = 0;

    if (*n == 0 || *lineptr == NULL) {
        *n = BUFFER_SIZE;
        *lineptr = malloc(*n);
        if (*lineptr == NULL) {
            return -1; // Memory allocation error
        }
    }

    while (1) {
        if (buffer_pos >= buffer_size) {
            buffer_size = read(fileno(stream), buffer, BUFFER_SIZE);
            if (buffer_size == 0) {
                end_of_file = 1;
                break; // End of file reached
            }
            buffer_pos = 0;
        }

        if (buffer_size == -1) {
            return -1; // Read error
        }

        char c = buffer[buffer_pos++];
        
        if (line_length + 1 >= *n) {
            *n *= 2;
            char *new_lineptr = realloc(*lineptr, *n);
            if (new_lineptr == NULL) {
                return -1; // Memory allocation error
            }
            *lineptr = new_lineptr;
        }

        (*lineptr)[line_length++] = c;

        if (c == '\n') {
            break; // Line read successfully
        }
    }

    if (line_length > 0) {
        (*lineptr)[line_length] = '\0';
        return line_length;
    } else if (end_of_file) {
        return -1; // End of file reached
    } else {
        return 0; // No characters read
    }
}

int main(void) {
    char *line = NULL;
    size_t len = 0;
    
    printf("Enter a line: ");
    ssize_t bytes_read = getline_(&line, &len, stdin);

    if (bytes_read > 0) {
        printf("Read %zd bytes: %s", bytes_read, line);
    } else if (bytes_read == 0) {
        printf("No characters read.\n");
    } else {
        printf("End of file reached or read error.\n");
    }

    free(line);
    return 0;
}
