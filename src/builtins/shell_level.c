/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_level.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 16:08:07 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/16 18:44:33 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	env_list_size(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

char	*create_env_entry(t_env *node)
{
	char	*temp;
	char	*joined;

	temp = ft_strjoin(node->name, "=");
	if (!temp)
		return (NULL);
	joined = ft_strjoin(temp, node->val);
	free(temp);
	return (joined);
}

char	**convert_to_envp(t_env *env_list)
{
	char	**envp;
	int		i;

	i = 0;
	envp = malloc(sizeof(char *) * (env_list_size(env_list) + 1));
	if (!envp)
		return (NULL);
	while (env_list)
	{
		envp[i] = create_env_entry(env_list);
		if (!envp[i])
		{
			while (i > 0)
				free(envp[--i]);
			free(envp);
			return (NULL);
		}
		i++;
		env_list = env_list->next;
	}
	envp[i] = NULL;
	return (envp);
}
