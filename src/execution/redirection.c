/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 20:44:04 by xueyang           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/04/16 13:21:13 by xueyang          ###   ########.fr       */
=======
/*   Updated: 2025/04/16 13:24:54 by jwardeng         ###   ########.fr       */
>>>>>>> 7eb06e40badc137b8c1355ce39f6768908022d88
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// redirect output overwrite>
int	red_out_overwrite(char *filename)
{
	int fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		perror("open error");
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

// redirect output append >>
int	red_out_append(char *filename)
{
	int fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		perror("open error");
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

// redirect input <
int	red_in(char *filename)
{
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		perror("open error");
	dup2(fd, STDIN_FILENO);
	close(fd);
}
