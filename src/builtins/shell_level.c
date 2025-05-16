/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_level.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 16:08:07 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/16 17:17:31 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_strjoin3(char *s1, char *s2, char *s3)
{
	char	*res;
	int		len1 = ft_strlen(s1);
	int		len2 = ft_strlen(s2);
	int		len3 = ft_strlen(s3);
	int		i = 0, j = 0;

	res = malloc(len1 + len2 + len3 + 1);
	if (!res)
		return (NULL);
	while (i < len1)
		res[i++] = *s1++;
	while (j < len2)
		res[i++] = *s2++;
	j = 0;
	while (j < len3)
		res[i++] = *s3++;
	res[i] = '\0';
	return (res);
}

int	env_list_size(t_env *env)
{
	int	count = 0;

	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

char	**convert_to_envp(t_env *env_list)
{
	char	**envp;
	char	*joined;
	char	*temp;
	int		i = 0;

	envp = malloc(sizeof(char *) * (env_list_size(env_list) + 1));
	if (!envp)
		return (NULL);
	while (env_list)
	{
		temp = ft_strjoin(env_list->name, "=");
		joined = ft_strjoin(temp, env_list->val);
		free(temp);
		if (!joined)
		{
			while (i > 0)
				free(envp[--i]);
			free(envp);
			return (NULL);
		}
		envp[i] = joined;
		// printf("%s\n", envp[i]);
		i++;
		env_list = env_list->next;
	}
	envp[i] = NULL;
	return (envp);
}

int	shell_level(t_token_data *td)
{
	int		shlvl;
	char	*shlvl_str;
	t_env	*node;

	shlvl_str = search_name_val(td->env_list, "SHLVL");
	node = search_name_node(td->env_list, "SHLVL");
	if (shlvl_str == NULL || ft_ministrcmp(shlvl_str, "") == 0)
		shlvl = 1;
	else
	{
		shlvl = ft_atoi(shlvl_str);
		if (shlvl < 0)
			shlvl = 0;
		else
			shlvl++;
	}
	if (node == NULL)
	{
		node = create_env("SHLVL", ft_env_strdup(ft_itoa(shlvl), td->gc, ENV), td->gc);
		if (!node)
			return (error_general("malloc: env not initiated\n"));
		ft_env_add_back(&td->env_list, node);
	}
	else
		node->val = ft_env_strdup(ft_itoa(shlvl), td->gc, ENV);
	return (0);
}
