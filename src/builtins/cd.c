/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:18:21 by xueyang           #+#    #+#             */
/*   Updated: 2025/03/30 15:19:27 by xueyang          ###   ########.fr       */
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

int	swap_PWDs(t_env *top)
{
	t_env	*pwd_node;
	t_env	*old_pwd_node;
	char	*pwd;
	char	*old_pwd;

	pwd_node = search_by_name(&top, "PWD");
	pwd =  ft_strdup(pwd_node->val);
	if (!pwd)
		return (1);
	old_pwd_node = search_by_name(&top, "OLDPWD");
	old_pwd =  ft_strdup(old_pwd_node->val);
	if (!old_pwd)
		return (1);
	if (update_env_var(&top, "OLDPWD", pwd) == 1)
		return (1);
	if (update_env_var(&top, "PWD", old_pwd) == 1)
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
		if (path[i] = '/')
			return (i);
		i--;
	}
	return (-1);
}

char	*get_parent_dir(char *cur_dir)
{
	int		len;
	int		i;
	char	*pwd;
	char	*temp;

	pwd = ft_strdup(cur_dir);
	if (!pwd)
		return (NULL);
	len = strlen(pwd);
	while (len > 1 && pwd[len - 1] == '/')
		pwd[--len] = '\0';
	i = find_last_slash(pwd);
	if (i == 0)
	{
		free(pwd);
		pwd = ft_strdup("/"); // no need to protect because it's returning NULL anyways
	}
	else
	{
		temp = pwd;
		pwd = ft_substr(temp, 0, i + 1);
		free(temp);
	}
	return (pwd);
}

int	cd_tilde(t_env *top, char *path, char *home)
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
	return (0);
}

int	cd_nothing(t_env *top, char *home)
{
	if (chdir(home) == -1)
		return (error_general("chdir: failed to go home dir"));
	if (update_PWDs(top, home) == 1)
		return (error_general("malloc: failed to update path"));
	return (0);
}

// int	cd_normal(t_env *top, char *path)
// {
// 	if (chdir(path) == -1)
// 		return (error_general("cd: no such file or directory"));
// 	if (update_PWDs(top, path) == 1)
// 		return (error_general("malloc: failed to update path"));
// 	return (0);
// }

int	cd_minus(t_env *top, char *path, char *home)
{
	if (!path[1])
	{
		if (chdir(search_name_val(&top, "OLDPWD")) == -1)
			return (error_general("OLDPWD not set"));
		if (swap_PWDs(top) == 1)
			return (error_general("malloc: failed to update path"));
	}
	else if (path[1] == '-')
	{
		if (chdir(home) == -1)
			return (error_general("chdir: failed to go home dir"));
		if (update_PWDs(top, home) == 1)
			return (error_general("malloc: failed to update path"));
	}
	else
		return (error_general("cd: invalid option"));
	return (0);
}

// int	count_dot(char *path)
// {
// 	int i;
// 	int	count_parent;
// 	int	count_self;

// 	i = 0;
// 	count_parent = 0;
// 	count_self = 0;
// 	while(path[i])
// 	{
// 		if (path[i] == '.' && path[i + 1] == '.' && path[i + 2] == '/')
// 		{
// 			i = i + 2;
// 			count_parent++;
// 		}
// 		else if (path[i] == '.' && path[i + 1] == '/')
// 		{
// 			i++;
// 			count_self++;
// 		}
// 		i++;
// 	}
// 	return (count_parent);
// }

// int	cd_dot(t_env *top, char *path)
// {
// 	t_env	*pwd_node;
// 	int		i;


// 	pwd_node = search_by_name(&top, "PWD");
// 	while(path[i])
// 	{
// 		if (path[i] == '.' && path[i + 1] == '.' && path[i + 2] == '/')
// 		{
// 			i = i + 2;
			
// 		}
// 	}
// }

char	*normalize_path(const char *path)
{
	char	*full_path;
	char	*current_path;
	char	**tokens;
	char	*temp;
	int		i;

	if (path[0] == '/')
		full_path = ft_strdup("/");
	else
		full_path = getcwd(NULL, 0);
	if (!full_path)
		return (NULL);
	tokens = ft_split(path, '/');
	if (!tokens)
	{
		free(full_path);
		return (NULL);
	}
	current_path = full_path;
	i = 0;
	while (tokens[i])
	{
		if (ft_strcmp(tokens[i], "..") == 0)
		{
			temp = get_parent_dir(current_path);
			free(current_path);
			current_path = temp;
		}
		else if (ft_strcmp(tokens[i], ".") != 0 && tokens[i][0] != '\0')
		{
			if (ft_strlen(current_path) > 1 && \
				current_path[ft_strlen(current_path) - 1] != '/')
			{
				temp = ft_strjoin(current_path, "/");
				free(current_path);
				current_path = temp;
			}
			temp = ft_strjoin(current_path, tokens[i]);
			free(current_path);
			current_path = temp;
		}
		free(tokens[i]);
		i++;
	}
	free(tokens);
	if (!current_path || ft_strlen(current_path) == 0)
	{
		free(current_path);
		current_path = ft_strdup("/");
	}
	return (current_path);
}

int	ft_cd(t_token *current, t_env *top)
{
	char	*path;
	char	*home;
	char	*temp;
	
	home = find_home();
	if (!home)
		return (1);
	if (!current->next)
		return (cd_nothing(top, home));
	else
	{
		path = current->next;
		if (path[0] == '~')
			return (cd_tilde(top, path, home) == 1);
		else if (path[0] == '-')
			return (cd_minus(top, path, home));
		else
		{
			temp = path;
			path = normalize_path(temp);
			// free(temp);
			if (chdir(path) == -1)
				return (error_general("cd: no such file or directory"));
			if (update_PWDs(top, path) == 1)
				return (error_general("malloc: failed to update path"));
		}
	}
	return (0);
}