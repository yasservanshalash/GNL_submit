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

void	leftover_logic(char **line, char **leftover, int *i, int *j)
{
	char	*temp;

	if (*leftover != NULL)
	{
		while ((*leftover)[*j] != '\n' && (*leftover)[*j] != '\0')
		{
			(*line)[(*i)++] = (*leftover)[(*j)++];
		}
		if ((*leftover)[*j] == '\n')
		{
			temp = ft_strdup(*leftover + *j + 1);
			free(*leftover);
			*leftover = temp;
			(*line)[(*i)++] = '\n';
			(*line)[*i] = '\0';
		}
		else
		{
			free(*leftover);
			*leftover = NULL;
		}
	}
}

char	*leftover_in_line_logic(char **leftover, char **line, int *j)
{
	*leftover = ft_strdup(*line + *j + 1);
	if (*leftover == NULL)
	{
		free(*line);
		return (NULL);
	}
	(*line)[(*j)++] = '\n';
	(*line)[*j] = '\0';
	return (*leftover);
}

char	*line_logic(int fd, char **line, int *i, int *j)
{
	int		read_chars;
	char	*leftover;

	read_chars = read(fd, *line + *i, BUFFER_SIZE - *i);
	while (read_chars > 0)
	{
		*i += read_chars;
		if (ft_strchr(*line, '\n'))
		{
			*j = *i - read_chars;
			while (*j < *i && (*line)[*j] != '\n')
				(*j)++;
			if (*j < *i)
			{
				return (leftover_in_line_logic(&leftover, line, j));
			}
		}
		*line = ft_realloc(*line, *i + BUFFER_SIZE + 1, (*i + BUFFER_SIZE + 1) * sizeof(char));		if (*line == NULL)
			return (NULL);
		ft_bzero(*line + *i, BUFFER_SIZE + 1);
		read_chars = read(fd, *line + *i, BUFFER_SIZE);
	}
	return (NULL);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*leftover = NULL;
	int			i;
	int			j;

	i = 0;
	j = 0;
	if (fd < 0)
		return (NULL);
	line = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	ft_bzero(line, BUFFER_SIZE + 1);
	leftover_logic(&line, &leftover, &i, &j);
	if (ft_strchr(line, '\n'))
	{
		return (line);
	}
	leftover = line_logic(fd, &line, &i, &j);
	if (i > 0)
		return (line);
	else
	{
		free(line);
		return (NULL);
	}
}

// int main(void) {
//     int fd;
//     fd = open("text.txt", O_RDONLY);
//     if (fd < 0) {
//         perror("Error opening file");
//         return (1);
//     }

//     char *result;
//     while ((result = get_next_line(fd)) != NULL) {
//         printf("%s", result);
//         free(result);  // Free each line obtained from get_next_line
//     }

//     close(fd);
//     return (0);
// }