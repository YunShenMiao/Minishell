/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:18:21 by xueyang           #+#    #+#             */
/*   Updated: 2025/03/24 15:46:03 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// cd haha >> output.txt 
// will print out the error message insetad of writing to output.txt, the .txt file will still be generated
// pwd is actually similar??
// needs to be checked again

char	*find_home(void)
{
	char	*home;

	home = getenv("HOME");
	if (!home)
	{
		perror("HOME not set\n");
		return (NULL);
	}
	return (home);
}

int	update_PWDs(t_env *top, char *new_pwd)
{
	t_env	*pwd_node;
	char	*old_pwd;

	pwd_node = search_by_name(&top, "PWD");
	old_pwd =  pwd_node->val;
	if (update_env_var(&top, "OLDPWD", old_pwd) == 1)
		return (1);
	if (update_env_var(&top, "PWD", new_pwd) == 1)
		return (1);
	return (0);
}

int	cd_tilde(t_env *top, char *path, char *home)
{
	char	*full_path;
	char	*use_part;

	use_part = ft_substr(path, 1, ft_strlen(path) - 1);
	if (!use_part)
		return (error_general("malloc: failed to create path"));
	full_path = ft_strjoin(home, use_part);
	if (!full_path)
	{
		free(use_part);
		return (error_general("malloc: failed to create path"));
	}
	free(use_part);
	if (chdir(full_path) == -1)
	{
		free(full_path);
		return (error_general("malloc: failed to create path"));		
	}
	if (update_PWDs(top, full_path) == 1)
	{
		free(full_path);
		return (error_general("malloc: failed to update path"));
	}
	free(full_path);
}

// need to update PWD && OLDPWD
int	ft_cd(t_token *current, t_env *top)
{
	char	*path;
	char	*home;
	
	home = find_home();
	if (current->next)
	{
		path = current->next;
		if (path[0] == '~')
		{
			if (!home)
				return (1);
			if (!path[1])
			{
				if (chdir(home) == -1)
				return (1);
			}
			else if (cd_tilde(top, path, home) == 1)
				return (1);
		}
		if (path[0] == '-')
		{
			if (!path[1])
			{
				if (chdir(search_name_val(&top, "OLDPWD")) == -1)
					return (error_general("OLDPWD not set"));
			}
			else if (path[1] == '-')
			{
				if (chdir(home) == -1)
					return (1);
			}
			else
				return (error_general("cd: invalid option"));
		}
		if (chdir(path) == -1)
			return (error_general("cd: no such file or directory"));
	}
	else
		if (chdir(home) == -1)
			return (1);
	return (0);
}