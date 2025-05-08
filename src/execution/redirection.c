/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 20:44:04 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/08 19:41:49 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	red_out_overwrite(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (perror("open error"), 1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (1);
	close(fd);
	return (0);
}

int	red_out_append(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return (perror("open error"), 1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (1);
	close(fd);
	return (0);
}

int	red_in(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (perror("open error"), 1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (1);
	close(fd);
	return (0);
}
