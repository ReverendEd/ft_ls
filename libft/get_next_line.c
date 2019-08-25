/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsehr <tsehr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/30 19:37:41 by tsehr             #+#    #+#             */
/*   Updated: 2019/07/03 22:51:50 by tsehr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "get_next_line.h"

int		get_next_line(const int fd, char **line)
{
	static char	*pool[255];
	int			line_length;
	int			buffer_status;

	REQUIRE((fd >= 0 && line));
	if (!pool[fd])
		pool[fd] = ft_strnew(50);
	line_length = save_line_and_return_index(pool, fd, line);
	if (pool[fd][line_length] == '\0')
	{
		buffer_status = buffer_pool(pool, fd, line_length, 0);
		if (buffer_status == -1)
			return (-1);
		else if (buffer_status == 1 && !pool[fd][1])
			return (0);
		line_length = save_line_and_return_index(pool, fd, line);
		clean_pool(pool, fd, line_length);
		return (1);
	}
	else if (pool[fd][line_length] == '\n')
	{
		clean_pool(pool, fd, line_length);
		return (1);
	}
	return (-1);
}

void	clean_pool(char *pool[255], int fd, int line_end)
{
	int		pool_start;
	int		next_line_start;
	char	*temp;

	next_line_start = line_end + 1;
	temp = ft_strnew(line_end + BUFF_SIZE);
	pool_start = 0;
	while (pool[fd][next_line_start])
	{
		temp[pool_start] = pool[fd][next_line_start];
		pool_start++;
		next_line_start++;
	}
	temp[pool_start] = '\0';
	ft_bzero(pool[fd], ft_strlen(pool[fd]));
	pool[fd] = ft_strcpy(pool[fd], temp);
	free(temp);
}

int		save_line_and_return_index(char *pool[255], int fd, char **line)
{
	int	line_length;

	line_length = 0;
	while (pool[fd][line_length] != '\n')
	{
		if (pool[fd][line_length] == '\0')
		{
			return (line_length);
		}
		line_length++;
	}
	*line = ft_strnew(line_length + 1);
	*line = ft_strncpy(*line, pool[fd], line_length);
	return (line_length);
}

int		buffer_pool(char *pool[9973], int fd, int pool_index, int v_flag)
{
	char	buffer[BUFF_SIZE + 1];
	int		pool_length;
	int		buffer_index;
	int		buffer_length;

	pool_length = ft_strlen(pool[fd]);
	while (!v_flag)
	{
		buffer_index = 0;
		if ((buffer_length = read(fd, buffer, BUFF_SIZE)) == 0)
			return (v_flag = handle_file_end(pool, fd, pool_index));
		REQUIRE((buffer_length != -1));
		pool[fd] = ft_extend_string(pool[fd], (size_t)buffer_length);
		pool_length += buffer_length;
		while (pool_index < pool_length)
		{
			pool[fd][pool_index] = buffer[buffer_index];
			if (buffer[buffer_index] == '\n')
				v_flag = 1;
			pool_index++;
			buffer_index++;
		}
	}
	pool[fd][pool_index] = '\0';
	return (0);
}

int		handle_file_end(char *pool[9973], int fd, int pool_index)
{
	if (ft_strlen(pool[fd]) == 0)
	{
		ft_bzero(pool[fd], ft_strlen(pool[fd]));
		return (1);
	}
	pool[fd][pool_index] = '\n';
	pool_index++;
	pool[fd][pool_index] = '\0';
	return (1);
}
