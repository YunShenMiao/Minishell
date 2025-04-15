/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 20:44:12 by xueyang           #+#    #+#             */
/*   Updated: 2025/04/15 16:11:40 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_child(char *path, char **arr_cmd, char **envp)
{
	if (path == NULL)
	{
		put_path_error(arr_cmd[0]);
		free_array(arr_cmd);
		exit(127);
	}
	if (access(path, X_OK) == -1)
	{
		free_array(arr_cmd);
		perror("permission denied");
		exit(126);
	}
	if (execve(path, arr_cmd, envp) == -1)
	{
		free_array(arr_cmd);
		exit_handling("execve error");
	}
	exit(0);
}

void	child_one(int *fd, int in, char **envp, char *cmd)
{
	char	**arr_cmd;
	char	*path;

	if (in == 1)
		exit (EXIT_FAILURE);
	close(fd[0]);
	dup2(in, STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	arr_cmd = parse_command_result(cmd);
	if (arr_cmd == NULL)
		exit_handling("malloc error");
	path = find_path(arr_cmd[0], envp);
	check_child(path, arr_cmd, envp);
}

void	child_two(int *fd, int out, char **envp, char *cmd)
{
	char	**arr_cmd;
	char	*path;

	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	dup2(out, STDOUT_FILENO);
	close(fd[0]);
	arr_cmd = parse_command_result(cmd);
	if (arr_cmd == NULL)
		exit_handling("malloc error");
	path = find_path(arr_cmd[0], envp);
	check_child(path, arr_cmd, envp);
}