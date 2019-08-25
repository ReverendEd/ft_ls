/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsehr <tsehr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/30 19:58:35 by tsehr             #+#    #+#             */
/*   Updated: 2019/07/05 23:30:03 by tsehr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# define BUFF_SIZE 2056

# define REQUIRE(x) if (!x) return (-1)

# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <fcntl.h>
# include "libft.h"

int		get_next_line(const int fd, char **line);
int		buffer_pool(char *pool[255], int fd, int pool_index, int v_flag);
int		save_line_and_return_index(char *pool[255], int fd, char **line);
int		handle_file_end(char *pool[255], int fd, int pool_index);
void	clean_pool(char *pool[255], int fd, int line_length);
char	*ft_strnew(size_t size);
char	*ft_strncpy(char *dest, const char *src, size_t n);
size_t	ft_strlen(const char *s);

#endif
