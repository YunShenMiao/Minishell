/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:21:43 by xueyang           #+#    #+#             */
/*   Updated: 2025/03/31 12:30:04 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_export(t_env	*top_env)
{
	t_env	*temp;

	temp = top_env;
	while (temp->next)
	{
		printf("declare -x %s=%s\n", temp->name, temp->val);
		temp = temp->next;
	}
	printf("declare -x %s=%s\n", temp->name, temp->val);
	return (0);
}

int	add_env_var(t_env *top_env, char *assign)
{
	t_env	*new;
	char	*name;
	char	*value;

	name = ft_substr(assign, 0, find_sign(assign, '='));
	if (!name)
		return (error_general("malloc: env not initiated"));
	value = ft_substr(assign, find_sign(assign, '=') + 1, ft_strlen(assign));
	if (!value)
		return (error_general("malloc: env not initiated"));
	new = create_env(name, value);
	if (!new)
		return (error_general("malloc: env not initiated"));
	ft_env_add_back(&top_env, new);
	return (0);
}

int	ft_export(t_env	*top_env, t_token *current)
{
	char	*assign;
	t_env	*new;


	if (!current->next)
		print_export(top_env);
	else if (current->next->next)
		return (error_general("export: bad assign"));
	else
	{
		assign = current->next->value;
		if (find_sign(assign, '=') < 0)
		{
			new = create_env(assign, NULL);
			if (!new)
				return (error_general("malloc: env not initiated"));
			ft_env_add_back(&top_env, new);
		}
		else if (find_sign(assign, '=') == 0)
			return (error_general("export: not a valid identifier"));
		else
			return (add_env_var(top_env, assign));
	}
	return (0);
}