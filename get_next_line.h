/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: yasser <marvin@42.fr>                        +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/23 15:18:31 by yasser        #+#    #+#                 */
/*   Updated: 2023/12/15 12:02:00 by yshalash      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

char	*get_next_line(int fd);
char	*ft_strdup(const char *src);
const char	*ft_strchr(const char *s, int c);
void	ft_bzero(void *s, size_t n);
void	*ft_realloc(void *ptr, size_t prev_size, size_t new_size);

#endif
