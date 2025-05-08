/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_envp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:00:31 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/08 17:27:49 by jwardeng         ###   ########.fr       */
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

t_env	*init_env(char **envp, t_gc *gc)
{
	t_env	*top_env;
	t_env	*new;
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
		name = ft_env_substr(envp[i], 0, find_sign(envp[i], '='), gc);
		if (!name)
			return (NULL);
		value = ft_env_substr(envp[i], find_sign(envp[i], '=') + 1,
				ft_strlen(envp[i]), gc);
		if (!value)
			return (NULL);
		new = create_env(name, value, gc);
		if (!new)
			return (NULL);
		ft_env_add_back(&top_env, new);
		i++;
	}
	return (top_env);
}
