#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1000000
#endif

void leftover_logic(char **line, char **leftover, int *i, int *j) {
    if (*leftover != NULL) {
        while ((*leftover)[*j] != '\n' && (*leftover)[*j] != '\0') {
            (*line)[(*i)++] = (*leftover)[(*j)++];
        }
        if ((*leftover)[*j] == '\n') {
            *leftover = strdup(*leftover + *j + 1);
            (*line)[(*i)++] = '\n';
            (*line)[*i] = '\0';
        }
        // Handle case where leftover doesn't contain a newline
        if ((*leftover)[*j] == '\0') {
            free(*leftover);
            *leftover = NULL;
        }
    }
}

char *line_logic(int fd, char **line, int *i, int *j) {
    int read_chars;
    char *leftover;
    char *new_line;

    read_chars = read(fd, *line + *i, BUFFER_SIZE - *i);
    while (read_chars > 0) {
        *i += read_chars;
        if (strchr(*line, '\n')) {
            *j = *i - read_chars;
            while (*j < *i && (*line)[*j] != '\n') {
                (*j)++;
            }
            if (*j < *i) {
                leftover = strdup(*line + *j + 1);
                (*line)[(*j)++] = '\n';
                (*line)[*j] = '\0';
                return leftover;
            }
        }
        // Replace realloc with free and malloc
        new_line = malloc((*i + BUFFER_SIZE + 1) * sizeof(char));
        if (new_line == NULL) {
            free(*line);  // Free the original line buffer
            return NULL;
        }
        memcpy(new_line, *line, *i);
        free(*line);
        *line = new_line;
        memset(*line + *i, 0, BUFFER_SIZE + 1);
        read_chars = read(fd, *line + *i, BUFFER_SIZE);
    }
    return NULL;
}


char *get_next_line(int fd) {
    char *line;
    static char *leftover = NULL;
    int i = 0;
    int j = 0;

    if (fd < 0)
        return NULL;
    line = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
    bzero(line, BUFFER_SIZE + 1);
    if (line == NULL)
        return NULL;
    leftover_logic(&line, &leftover, &i, &j);
    if (strchr(line, '\n'))
        return line;
    leftover = line_logic(fd, &line, &i, &j);
    if (i > 0) {
        line[i] = '\0';
        return line;
    } else {
        free(line);
        return NULL;
    }
}


// int main(void)
// {
//     int fd;

//     fd = open("text.txt", O_RDONLY);
//     printf("%s", get_next_line(fd));
//     printf("%s", get_next_line(fd));
//     printf("%s", get_next_line(fd));
//     printf("%s", get_next_line(fd));
//     printf("%s", get_next_line(fd));
//     printf("%s", get_next_line(fd));
//     printf("%s", get_next_line(fd));
//     close(fd);
//     return (0);
// }
