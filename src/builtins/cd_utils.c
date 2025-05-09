/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 17:45:08 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/09 16:30:42 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*find_home(t_env *top)
{
	char	*home;

	home = search_name_val(top, "HOME");
	if (!home)
	{
		perror("HOME not set\n");
		return (NULL);
	}
	return (home);
}

int	update_pwds(t_env *top, char *new_pwd, t_gc *gc)
{
	t_env	*pwd_node;
	char	*old_pwd;

	pwd_node = search_name_node(top, "PWD");
	old_pwd = ft_env_strdup(pwd_node->val, gc, BUILT_IN);
	if (!pwd_node || !pwd_node->val)
		return (error_general("PWD not set"));
	if (update_env_var(&top, "OLDPWD", old_pwd, gc) == 1)
		return (1);
	if (update_env_var(&top, "PWD", new_pwd, gc) == 1)
		return (1);
	return (0);
}

int	swap_pwds(t_env *top, t_gc *gc)
{
	t_env	*pwd_node;
	t_env	*old_pwd_node;
	char	*pwd;
	char	*old_pwd;

	pwd_node = search_name_node(top, "PWD");
	if (!pwd_node || !pwd_node->val)
		return (error_general("PWD not set"));
	pwd = ft_env_strdup(pwd_node->val, gc, BUILT_IN);
	if (!pwd)
		return (1);
	old_pwd_node = search_name_node(top, "OLDPWD");
	old_pwd = ft_env_strdup(old_pwd_node->val, gc, BUILT_IN);
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

	pwd = ft_env_strdup(cur_dir, gc, BUILT_IN);
	if (!pwd)
		return (NULL);
	len = ft_strlen(pwd);
	while (len > 1 && pwd[len - 1] == '/')
		pwd[--len] = '\0';
	i = find_last_slash(pwd);
	if (i == 0)
		pwd = ft_env_strdup("/", gc, BUILT_IN);
	else
	{
		temp = pwd;
		pwd = ft_env_substr(temp, 0, i + 1, gc);
	}
	return (pwd);
}
