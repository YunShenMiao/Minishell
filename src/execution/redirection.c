/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 20:44:04 by xueyang           #+#    #+#             */
/*   Updated: 2025/04/30 12:10:12 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// redirect output overwrite>
int	red_out_overwrite(char *filename)
{
	int fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return(perror("open error"), 1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (1);
	close(fd);
	return (0);
}

// redirect output append >>
int	red_out_append(char *filename)
{
	int fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return(perror("open error"), 1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (1);
	close(fd);
	return (0);
}

// redirect input <
int	red_in(char *filename)
{
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (perror("open error"), 1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (1);
	close(fd);
	return (0);
}

int heredoc(char *delimiter)
{
	int fd[2];
	char *line;

	if (pipe(fd) == -1)
	{
		perror("pipe error");
		return (-1);
    }
	while (1)
	{
		line = readline("> ");
		if (!line || (ft_strelen(line) == ft_strlen(delimiter) && ft_strncmp(line, delimiter, ft_strlen(line)) == 0))
		{
			free(line);
			break;
		}
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	close(fd[1]);
	return (fd[0]);
}
