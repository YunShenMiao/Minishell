/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:21:43 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/16 15:35:12 by xueyang          ###   ########.fr       */
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

int	add_env_var(t_env *top_env, char *assign, t_gc *gc)
{
	t_env	*new;
	char	*name;
	char	*value;
	int		loc;

	loc = find_sign(assign, '=');
	if (loc < 0)
		return (error_general("add_env_var: missing '=' in assignment\n"));
	if (assign[loc - 1] == '+')
		name = ft_env_substr(assign, 0, loc - 1, gc);
	else
		name = ft_env_substr(assign, 0, loc, gc);
	if (!name)
		return (error_general("malloc: env not initiated\n"));
	value = ft_env_substr(assign, loc + 1, ft_strlen(assign), gc);
	if (!value)
		return (error_general("malloc: env not initiated\n"));
	if (search_name_node(top_env, name))
	{
		new = search_name_node(top_env, name);
		new->val = value;
	}
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

int	export_helper(t_env	*top_env, char *arg, t_token_data *td)
{
	int		loc;
	char	*assign;
	t_env	*new;
	char	*temp;
	char	*name;

	if (is_valid(arg) < 0)
		return (error_general("export: not a valid identifier\n"));
	assign = arg;
	loc = find_sign(assign, '=');
	if (loc < 0)
	{
		if (!search_name_node(top_env, assign))
		{
			name = ft_env_substr(assign, 0, ft_strlen(assign), td->gc);
			if (!name)
				return (error_general("malloc: env not initiated\n"));
			new = create_env(name, NULL, td->gc);
			if (!new)
				return (error_general("malloc: env not initiated\n"));
			ft_env_add_back(&top_env, new);
		}
	}
	else if (loc == (int)ft_strlen(assign) || loc == 0)
		return (error_general("export: not a valid identifier\n"));
	else if (arg[loc - 1] == '+')
	{
		name = ft_env_substr(assign, 0, loc - 1, td->gc);
		new = search_name_node(top_env, name);
		if (new)
		{
			temp = ft_env_substr(assign, loc + 1, ft_strlen(assign) - loc - 1, td->gc);
			new->val = ft_env_strjoin(new->val, temp, td->gc);
		}
		else
			add_env_var(top_env, assign, td->gc);
	}
	else
		add_env_var(top_env, assign, td->gc);
	return (0);
}

int	ft_export(t_env	*top_env, char **args, t_token_data *td)
{
	int		i;
	int		result;

	if (!args[1])
		print_export(top_env);
	else
	{
		i = 1;
		while (args[i])
		{
			if (ft_strncmp(args[i], "-", 1) == 0)
			{
				error_general("usage: export not supporting flags\n");
				return (2);
			}
			result = export_helper(top_env, args[i], td);
			if (result != 0)
				return (result);
			i++;
		}
	}
	return (0);
}
