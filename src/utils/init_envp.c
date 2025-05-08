/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_envp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:00:31 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/08 18:58:23 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_arr_row(char **arr)
{
	int	i;

	i = 0;
	while (arr[i] != NULL)
		i++;
	return (i);
}

int	find_sign(char *str, char c)
{
	int	i;

	i = 0;
	if (!str)
		return (-1);
	while (str[i] != '\0')
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

static int	append_env_variable(t_env **top_env, char *env_entry, t_gc *gc)
{
	t_env	*new;
	char	*name;
	char	*value;

	name = ft_env_substr(env_entry, 0, find_sign(env_entry, '='), gc);
	if (!name)
		return (1);
	value = ft_env_substr(env_entry, find_sign(env_entry, '=') + 1,
			ft_strlen(env_entry), gc);
	if (!value)
		return (1);
	new = create_env(name, value, gc);
	if (!new)
		return (1);
	ft_env_add_back(top_env, new);
	return (0);
}

t_env	*init_env(char **envp, t_gc *gc)
{
	t_env	*top_env;
	char	*name;
	char	*value;
	int		i;

	name = ft_env_substr(envp[0], 0, find_sign(envp[0], '='), gc);
	if (!name)
		return (NULL);
	value = ft_env_substr(envp[0], find_sign(envp[0], '=') + 1,
			ft_strlen(envp[0]), gc);
	if (!value)
		return (NULL);
	top_env = create_env(name, value, gc);
	if (!top_env)
		return (NULL);
	i = 1;
	while (i < count_arr_row(envp))
	{
		if (append_env_variable(&top_env, envp[i], gc))
			return (NULL);
		i++;
	}
	return (top_env);
}
