/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 18:38:27 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/16 18:39:21 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_export(t_env	*top_env)
{
	t_env	*temp;

	temp = top_env;
	while (temp->next)
	{
		if (temp->val && ft_ministrcmp(temp->val, "(null)") != 0)
			printf("declare -x %s=\"%s\"\n", temp->name, temp->val);
		else
			printf("declare -x %s\n", temp->name);
		temp = temp->next;
	}
	if (temp->val && ft_ministrcmp(temp->val, "(null)") != 0)
		printf("declare -x %s=\"%s\"\n", temp->name, temp->val);
	else
		printf("declare -x %s\n", temp->name);
}

int	extract_env_pair(char *assign, t_gc *gc, char **name, char **value)
{
	int	loc;

	loc = find_sign(assign, '=');
	if (loc < 0)
		return (error_general("add_env_var: missing '=' in assignment\n"));
	if (assign[loc - 1] == '+')
		*name = ft_env_substr(assign, 0, loc - 1, gc);
	else
		*name = ft_env_substr(assign, 0, loc, gc);
	if (!(*name))
		return (error_general("malloc: env not initiated\n"));
	*value = ft_env_substr(assign, loc + 1, ft_strlen(assign), gc);
	if (!(*value))
		return (error_general("malloc: env not initiated\n"));
	return (0);
}

int	add_env_var(t_env *top_env, char *assign, t_gc *gc)
{
	t_env	*new;
	char	*name;
	char	*value;

	if (extract_env_pair(assign, gc, &name, &value))
		return (1);
	new = search_name_node(top_env, name);
	if (new)
		new->val = value;
	else
	{
		new = create_env(name, value, gc);
		if (!new)
			return (error_general("malloc: env not initiated\n"));
		ft_env_add_back(&top_env, new);
	}
	return (0);
}

int	is_valid(char *arg)
{
	size_t	i;
	int		in_value;

	i = 0;
	in_value = 0;
	while (i < ft_strlen(arg))
	{
		if ((arg[i] == '+' && arg[i + 1] == '=') || arg[i] == 61)
			in_value = 1;
		if (i == 0)
		{
			if (in_value == 0 && ((arg[i] != 61 && arg[i] < 65) || \
			(arg[i] > 90 && arg[i] < 97 && arg[i] != 95) || arg[i] > 122))
				return (-1);
		}
		else
		{
			if (in_value == 0 && !(arg[i] >= 48 && arg[i] <= 57) && \
			((arg[i] != 61 && arg[i] < 65) || (arg[i] > 90 && arg[i] < 97 \
				&& arg[i] != 95) || arg[i] > 122))
				return (-1);
		}
		i++;
	}
	return (1);
}
