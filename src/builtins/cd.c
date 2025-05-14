/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:18:21 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/14 16:51:13 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	cd_tilde(t_env *top, char *path, char *home, t_gc *gc)
{
	char	*full_path;
	char	*use_part;

	if (!path[1])
	{
		if (chdir(home) == -1)
			return (error_general("chdir: failed to go home dir\n"));
	}
	else
	{
		use_part = ft_env_substr(path, 1, ft_strlen(path) - 1, gc);
		if (!use_part)
			return (error_general("malloc: failed to create path\n"));
		full_path = ft_env_strjoin(home, use_part, gc);
		if (!full_path)
		{
			return (error_general("malloc: failed to create path\n"));
		}
		if (chdir(full_path) == -1)
			return (error_general("malloc: failed to create path\n"));
		if (update_pwds(top, full_path, gc) == 1)
			return (error_general("malloc: failed to update path\n"));
	}
	return (0);
}

static int	cd_nothing(t_env *top, char *home, t_gc *gc)
{
	if (chdir(home) == -1)
		return (error_general("chdir: failed to go home dir\n"));
	if (update_pwds(top, home, gc) == 1)
		return (error_general("malloc: failed to update path\n"));
	return (0);
}

static int	cd_minus(t_env *top, char *path, char *home, t_gc *gc)
{
	if (!path[1])
	{
		if (chdir(search_name_val(top, "OLDPWD")) == -1)
			return (error_general("OLDPWD not set\n"));
		if (swap_pwds(top, gc) == 1)
			return (error_general("malloc: failed to update path\n"));
	}
	else if (path[1] == '-')
	{
		if (chdir(home) == -1)
			return (error_general("chdir: failed to go home dir\n"));
		if (update_pwds(top, home, gc) == 1)
			return (error_general("malloc: failed to update path\n"));
	}
	else
		return (error_general("cd: invalid option\n"));
	return (0);
}

static int	handle_cd_path(char *path, t_env *top, char *home, t_gc *gc)
{
	char	*normalized;

	if (path[0] == '~')
		return (cd_tilde(top, path, home, gc));
	else if (path[0] == '-')
		return (cd_minus(top, path, home, gc));
	normalized = normalize_path(path, gc);
	if (!normalized)
		return (error_general("malloc: path normalization failed\n"));
	if (chdir(normalized) == -1)
		return (error_general("cd: no such file or directory\n"));
	if (update_pwds(top, normalized, gc) == 1)
		return (error_general("malloc: failed to update path\n"));
	return (0);
}

int	ft_cd(char **args, t_env *top, t_gc *gc)
{
	char	*home;

	home = find_home(top);
	if (!home)
		return (1);
	if (!args[1])
		return (cd_nothing(top, home, gc));
	return (handle_cd_path(args[1], top, home, gc));
}
