/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:18:21 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/06 16:45:34 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// cd haha >> output.txt 
// will print out the error message insetad of writing to output.txt, the .txt file will still be generated
// pwd is actually similar??
// needs to be checked again

char	*find_home(t_env *top)
{
	char *home = search_name_val(top, "HOME");
	if (!home)
	{
		perror("HOME not set\n");
		return (NULL);
	}
	return (home);
}

int	update_PWDs(t_env *top, char *new_pwd, t_gc *gc)
{
	t_env	*pwd_node;
	char	*old_pwd;

	pwd_node = search_name_node(top, "PWD");
	old_pwd =  ft_env_strdup(pwd_node->val, gc);
	if (!pwd_node || !pwd_node->val)
		return (error_general("PWD not set"));
	if (update_env_var(&top, "OLDPWD", old_pwd, gc) == 1)
		return (1);
	if (update_env_var(&top, "PWD", new_pwd, gc) == 1)
		return (1);
	return (0);
}

int	swap_PWDs(t_env *top, t_gc *gc)
{
	t_env	*pwd_node;
	t_env	*old_pwd_node;
	char	*pwd;
	char	*old_pwd;

	pwd_node = search_name_node(top, "PWD");
	if (!pwd_node || !pwd_node->val)
		return (error_general("PWD not set"));
	pwd =  ft_env_strdup(pwd_node->val, gc);
	if (!pwd)
		return (1);
	old_pwd_node = search_name_node(top, "OLDPWD");
	old_pwd =  ft_env_strdup(old_pwd_node->val, gc);
	if (!old_pwd)
		return (1);
	if (update_env_var(&top, "OLDPWD", pwd, gc) == 1)
		return (1);
	if (update_env_var(&top, "PWD", old_pwd, gc) == 1)
		return (1);
	printf("%s\n", old_pwd);
	return (0);
}

int	find_last_slash(char *path)
{
	int	i;

	i = ft_strlen(path) - 1;
	while (i >= 0)
	{
		if (path[i] == '/')
			return (i);
		i--;
	}
	return (-1);
}

char	*get_parent_dir(char *cur_dir, t_gc *gc)
{
	int		len;
	int		i;
	char	*pwd;
	char	*temp;

	pwd = ft_env_strdup(cur_dir, gc);
	if (!pwd)
		return (NULL);
	len = ft_strlen(pwd);
	while (len > 1 && pwd[len - 1] == '/')
		pwd[--len] = '\0';
	i = find_last_slash(pwd);
	if (i == 0)
	{
		// free(pwd);
		pwd = ft_env_strdup("/", gc); // no need to protect because it's returning NULL anyways
	}
	else
	{
		temp = pwd;
		pwd = ft_env_substr(temp, 0, i + 1, gc);
		// free(temp);
	}
	return (pwd);
}

int	cd_tilde(t_env *top, char *path, char *home, t_gc *gc)
{
	char	*full_path;
	char	*use_part;

	if (!path[1])
	{
		if (chdir(home) == -1)
			return (error_general("chdir: failed to go home dir"));
	}
	else
	{
		use_part = ft_env_substr(path, 1, ft_strlen(path) - 1, gc);
		if (!use_part)
			return (error_general("malloc: failed to create path"));
		full_path = ft_env_strjoin(home, use_part, gc);
		if (!full_path)
		{
			// free(use_part);
			return (error_general("malloc: failed to create path"));
		}
		// free(use_part);
		if (chdir(full_path) == -1)
		{
			// free(full_path);
			return (error_general("malloc: failed to create path"));		
		}
		if (update_PWDs(top, full_path, gc) == 1)
		{
			// free(full_path);
			return (error_general("malloc: failed to update path"));
		}
		// free(full_path);
	}
	return (0);
}

int	cd_nothing(t_env *top, char *home, t_gc *gc)
{
	if (chdir(home) == -1)
		return (error_general("chdir: failed to go home dir"));
	if (update_PWDs(top, home, gc) == 1)
		return (error_general("malloc: failed to update path"));
	return (0);
}

int	cd_minus(t_env *top, char *path, char *home, t_gc *gc)
{
	if (!path[1])
	{
		if (chdir(search_name_val(top, "OLDPWD")) == -1)
			return (error_general("OLDPWD not set"));
		if (swap_PWDs(top, gc) == 1)
			return (error_general("malloc: failed to update path"));
	}
	else if (path[1] == '-')
	{
		if (chdir(home) == -1)
			return (error_general("chdir: failed to go home dir"));
		if (update_PWDs(top, home, gc) == 1)
			return (error_general("malloc: failed to update path"));
	}
	else
		return (error_general("cd: invalid option"));
	return (0);
}

char	*normalize_path(const char *path, t_gc *gc)
{
	char	*full_path;
	char	*current_path;
	char	**tokens;
	char	*temp;
	int		i;

	if (path[0] == '/')
		full_path = ft_env_strdup("/", gc);
	else
	{
		temp = getcwd(NULL, 0);
		if (!temp)
			return (NULL);
		full_path = ft_env_strdup(temp, gc);
		free(temp);
	}
	if (!full_path)
		return (NULL);
	tokens = ft_split(path, '/');
	if (!tokens)
	{
		// free(full_path);
		return (NULL);
	}
	current_path = full_path;
	i = 0;
	while (tokens[i])
	{
		if (ft_strncmp(tokens[i], "..", 3) == 0)
		{
			temp = get_parent_dir(current_path, gc);
			// free(current_path);
			current_path = temp;
		}
		else if (ft_strncmp(tokens[i], ".", 2) != 0 && tokens[i][0] != '\0')
		{
			if (ft_strlen(current_path) > 1 && \
				current_path[ft_strlen(current_path) - 1] != '/')
			{
				temp = ft_env_strjoin(current_path, "/", gc);
				// free(current_path);
				current_path = temp;
			}
			temp = ft_env_strjoin(current_path, tokens[i], gc);
			// free(current_path);
			current_path = temp;
		}
		free(tokens[i]);
		i++;
	}
	free(tokens);
	if (!current_path || ft_strlen(current_path) == 0)
	{
		// free(current_path);
		current_path = ft_env_strdup("/", gc);
	}
	return (current_path);
}

int	ft_cd(char **args, t_env *top, t_gc *gc)
{
	char	*path;
	char	*home;
	
	home = find_home(top);
	if (!home)
		return (1);
	if (!args[1])
		return (cd_nothing(top, home, gc));
	else
	{
		path = args[1];
		if (path[0] == '~')
			return (cd_tilde(top, path, home, gc));
		else if (path[0] == '-')
			return (cd_minus(top, path, home, gc));
		else
		{
			path = normalize_path(args[1], gc);
			if (!path)
				return (error_general("malloc: path normalization failed"));
			if (chdir(path) == -1)
			{
				// free(path);
				return (error_general("cd: no such file or directory"));
			}
			if (update_PWDs(top, path, gc) == 1)
			{
				// free(path);
				return (error_general("malloc: failed to update path"));
			}
			// free(path);
		}
	}
	return (0);
}