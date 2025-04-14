/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 10:47:20 by layang            #+#    #+#             */
/*   Updated: 2025/02/13 11:03:27 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* 
This version of get next line solved the problem of 1 byte still rechable
in res.
 */
static char	*get_line(char *res)
{
	char	*line;
	int		len;
	int		i;

	if (ft_strchr(res, '\n'))
		len = ft_strchr(res, '\n') - res + 1;
	else
		len = ft_strlen(res);
	line = (char *)malloc(len + 1);
	if (!line)
		return (NULL);
	i = 0;
	while (i < len)
	{
		line[i] = res[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

static char	*res_afterline(char *res)
{
	char	*new_res;

	if (ft_strchr(res, '\n'))
		new_res = ft_strdup(ft_strchr(res, '\n') + 1);
	else
		new_res = ft_strdup("");
	free(res);
	return (new_res);
}

static char	*read_line(int fd, char	**res, char	*buffer)
{
	ssize_t	bytes;
	char	*for_free;

	while (1)
	{
		if (ft_strchr(*res, '\n'))
			break ;
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes == -1)
			return (NULL);
		if (bytes == 0)
			break ;
		buffer[bytes] = '\0';
		for_free = *res;
		*res = ft_strjoin(for_free, buffer);
		free(for_free);
		if (!*res)
			return (NULL);
	}
	if (**res == '\0')
		return (NULL);
	return (get_line(*res));
}

/* char	*get_next_line(int fd)
{
	static char	*res;
	char		*line;
	char		*buffer;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = (char *)malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	if (!res)
		res = ft_strdup("");
	line = read_line(fd, &res, buffer);
	free(buffer);
	if (!line)
	{
		if (res)
			free(res);
		res = NULL;
		return (NULL);
	}
	res = res_afterline(res);
	return (line);
} */

void	free_res(int fd, char	**res)
{
	if (fd == -1 && *res)
	{
		if (*res)
			free(*res);
		*res = NULL;
	}
}

char	*get_next_line(int fd)
{
	static char	*res;
	char		*line;
	char		*buffer;

	free_res(fd, &res);
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = (char *)malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	if (!res)
		res = ft_strdup("");
	line = read_line(fd, &res, buffer);
	free(buffer);
	if (!line)
	{
		if (res)
			free(res);
		res = NULL;
		return (NULL);
	}
	res = res_afterline(res);
	return (line);
}
