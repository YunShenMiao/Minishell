/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 21:06:35 by xueyang           #+#    #+#             */
/*   Updated: 2025/03/23 13:30:33 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*fallback(char *cmd)
{
	int		i;
	char	*path;
	char	*full_path;
	char	*fallback_paths[3];

	i = 0;
	fallback_paths[0] = "/bin";
	fallback_paths[1] = "/usr/bin";
	fallback_paths[2] = NULL;
	while (fallback_paths[i])
	{
		path = ft_strjoin(fallback_paths[i], "/");
		full_path = ft_strjoin(path, cmd);
		free(path);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

static char	*handle_path(char *cmd, char *part_env)
{
	int		j;
	char	**paths;
	char	*path;
	char	*full_path;

	paths = ft_split(part_env + 5, ':');
	j = 0;
	while (paths[j])
	{
		path = ft_strjoin(paths[j++], "/");
		full_path = ft_strjoin(path, cmd);
		free(path);
		if (access(full_path, X_OK) == 0)
		{
			free_array(paths);
			return (full_path);
		}
		free(full_path);
	}
	free_array(paths);
	return (NULL);
}

char	*find_path(char *cmd, char **envp)
{
	int		i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '/')
			return (cmd);
		i++;
	}
	if (envp == NULL || !*envp)
		return (fallback(cmd));
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			return (handle_path(cmd, envp[i]));
		i++;
	}
	return (NULL);
}
