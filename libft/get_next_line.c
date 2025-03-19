/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 17:09:43 by xueyang           #+#    #+#             */
/*   Updated: 2025/02/05 20:05:11 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*extract_line_from_remain(char **remain)
{
	size_t	i;
	char	*this_line;
	char	*temp;

	if (*remain && find_new(*remain) >= 0)
	{
		i = find_new(*remain);
		this_line = ft_substr(*remain, 0, i + 1);
		if (!this_line)
			return (NULL);
		temp = *remain;
		*remain = ft_substr(temp, i + 1, ft_strlen(temp) - i - 1);
		free(temp);
		if (!*remain)
		{
			free(this_line);
			return (NULL);
		}
		return (this_line);
	}
	return (NULL);
}

char	*process_buffer(char **remain, char *buffer)
{
	char	*this_line;
	char	*temp;

	if (*remain == NULL)
	{
		*remain = ft_strdup(buffer);
		if (!*remain)
			return (NULL);
	}
	else
	{
		temp = *remain;
		*remain = ft_strjoin(temp, buffer);
		free(temp);
		if (!*remain)
			return (NULL);
	}
	this_line = extract_line_from_remain(remain);
	if (this_line)
		return (this_line);
	return (NULL);
}

char	*handle_end_conditions(ssize_t bytes_read, char **remain)
{
	char	*this_line;

	if (bytes_read < 0)
	{
		if (*remain)
		{
			free(*remain);
			*remain = NULL;
		}
		return (NULL);
	}
	if (*remain && **remain)
	{
		this_line = *remain;
		*remain = NULL;
		return (this_line);
	}
	if (*remain)
	{
		free(*remain);
		*remain = NULL;
	}
	return (NULL);
}

char	*read_and_process(int fd, char *buffer, char **remain)
{
	ssize_t	bytes_read;
	char	*this_line;

	bytes_read = read(fd, buffer, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		this_line = process_buffer(remain, buffer);
		if (this_line)
			return (this_line);
		bytes_read = read(fd, buffer, BUFFER_SIZE);
	}
	return (handle_end_conditions(bytes_read, remain));
}

char	*get_next_line(int fd)
{
	static char	*remain;
	char		*buffer;
	char		*this_line;

	if (fd >= 0 && BUFFER_SIZE > 0)
	{
		this_line = extract_line_from_remain(&remain);
		if (this_line)
			return (this_line);
		buffer = malloc(BUFFER_SIZE + 1);
		if (buffer)
		{
			this_line = read_and_process(fd, buffer, &remain);
			free(buffer);
			if (this_line)
				return (this_line);
		}
	}
	if (remain)
	{
		free(remain);
		remain = NULL;
	}
	return (NULL);
}
