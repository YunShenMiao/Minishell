/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:21:43 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/08 12:16:43 by xueyang          ###   ########.fr       */
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
}

int	add_env_var(t_env *top_env, char *assign, t_gc *gc)
{
	t_env	*new;
	char	*name;
	char	*value;


	name = ft_env_substr(assign, 0, find_sign(assign, '='), gc);
	if (!name)
		return (error_general("malloc: env not initiated"));
	value = ft_env_substr(assign, find_sign(assign, '=') + 1, ft_strlen(assign), gc);
	if (!value)
		return (error_general("malloc: env not initiated"));
	if (search_name_node(top_env, name))
	{
		new = search_name_node(top_env, name);
		new->val = value;
	}
	else
	{
		new = create_env(name, value, gc);
		if (!new)
			return (error_general("malloc: env not initiated"));
		ft_env_add_back(&top_env, new);
	}
	return (0);
}

// int	ft_export(t_env	*top_env, t_token *current, t_gc *gc)
// {
// 	char	*assign;
// 	t_env	*new;


// 	if (!current->next)
// 		print_export(top_env);
// 	else if (current->next->next)
// 		return (error_general("export: bad assign"));
// 	else
// 	{
// 		assign = current->next->value;
// 		if (find_sign(assign, '=') < 0)
// 		{
// 			new = create_env(assign, NULL, gc);
// 			if (!new)
// 				return (error_general("malloc: env not initiated"));
// 			ft_env_add_back(&top_env, new);
// 		}
// 		else if (find_sign(assign, '=') == 0)
// 			return (error_general("export: not a valid identifier"));
// 		else
// 			return (add_env_var(top_env, assign, gc));
// 	}
// 	return (0);
// }

int	ft_export(t_env	*top_env, char **args, t_gc *gc)
{
	char	*assign;
	t_env	*new;
	int		i;


	if (!args[1])
		print_export(top_env);
	else
	{
		i = 1;
		while (args[i])
		{
			if (ft_strncmp(args[i], "-", 1) == 0)
				return (error_general("usage: export not supporting flags"));
			if (is_numeric(args[i]) == 1 || find_sign(args[i], '-') != -1 )
				return (error_general("export: not a valid identifier"));
			assign = args[i];
			if (find_sign(assign, '=') < 0)
			{
				if (!search_name_node(top_env, assign))
				{
					new = create_env(assign, NULL, gc);
					if (!new)
						return (error_general("malloc: env not initiated"));
					ft_env_add_back(&top_env, new);
				}
			}
			else if (find_sign(assign, '=') == (int)ft_strlen(assign))
				return (error_general("export: not a valid identifier"));
			else if (find_sign(assign, '=') == 0)
				return (error_general("export: not a valid identifier"));
			else
				add_env_var(top_env, assign, gc);
			i++;
		}
	}
	return (0);
}