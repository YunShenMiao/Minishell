/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 21:06:35 by xueyang           #+#    #+#             */
/*   Updated: 2025/04/30 15:51:22 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*fallback(char *cmd, t_gc *gc)
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
		path = ft_env_strjoin(fallback_paths[i], "/", gc);
		full_path = ft_env_strjoin(path, cmd, gc);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		i++;
	}
	return (NULL);
}

static char	*handle_path(char *cmd, char *part_env, t_gc *gc)
{
	int		j;
	char	**paths;
	char	*path;
	char	*full_path;

	paths = ft_split(part_env + 5, ':');
	j = 0;
	while (paths[j])
	{
		path = ft_env_strjoin(paths[j++], "/", gc);
		full_path = ft_env_strjoin(path, cmd, gc);
		if (access(full_path, X_OK) == 0)
		{
			free_array(paths);
			return (full_path);
		}
	}
	free_array(paths);
	return (NULL);
}

char	*find_path(char *cmd, char **envp, t_gc *gc)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '/')
		{
			if (access(cmd, X_OK) == 0)
				return (cmd);
			return (NULL);
		}
		i++;
	}
	if (envp == NULL || !*envp)
		return (fallback(cmd, gc));
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			return (handle_path(cmd, envp[i], gc));
		i++;
	}
	return (NULL);
}
