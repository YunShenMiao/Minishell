/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:21:43 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/08 13:07:38 by xueyang          ###   ########.fr       */
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

int	is_valid(char *arg)
{
	size_t	i;
	int		in_value;;

	i = 0;
	in_value = 0;
	while (i < ft_strlen(arg))
	{
		if (arg[i] == 61)
			in_value = 1;
		if (arg[i] == '+' && arg[i + 1] == '=')
			in_value = 1;
		if (i == 0)
		{
			if (in_value == 0 && ((arg[i] != 61 && arg[i] < 65) || (arg[i] > 90 && arg[i] < 97 && arg[i] != 95) || arg[i] > 122))
				return (-1);
		}
		else
		{
			if (in_value == 0 && !(arg[i] >=48 && arg[i] <= 57) && ((arg[i] != 61 && arg[i] < 65) || (arg[i] > 90 && arg[i] < 97 && arg[i] != 95) || arg[i] > 122))
				return (-1);
		}

		i++;
	}
	return (1);
}

int	ft_export(t_env	*top_env, char **args, t_token_data *td)
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
			{
				error_general("usage: export not supporting flags");
				exit(2);
			}
			if (is_valid(args[i]) < 0)
				return (error_general("export: not a valid identifier"));
			assign = args[i];
			if (find_sign(assign, '=') < 0)
			{
				if (!search_name_node(top_env, assign))
				{
					new = create_env(assign, NULL, td->gc);
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
				add_env_var(top_env, assign, td->gc);
			i++;
		}
	}
	return (0);
}