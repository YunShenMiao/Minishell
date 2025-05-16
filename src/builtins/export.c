/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:21:43 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/16 18:42:36 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	export_no_equals(t_env *top_env, char *assign, t_token_data *td)
{
	t_env	*new;
	char	*name;

	if (search_name_node(top_env, assign))
		return (0);
	name = ft_env_substr(assign, 0, ft_strlen(assign), td->gc);
	if (!name)
		return (error_general("malloc: env not initiated\n"));
	new = create_env(name, NULL, td->gc);
	if (!new)
		return (error_general("malloc: env not initiated\n"));
	ft_env_add_back(&top_env, new);
	return (0);
}

int	export_append_mode(t_env *top_env, char *a, int loc, t_token_data *td)
{
	char	*name;
	char	*temp;
	t_env	*new;

	name = ft_env_substr(a, 0, loc - 1, td->gc);
	if (!name)
		return (error_general("malloc: env not initiated\n"));
	new = search_name_node(top_env, name);
	if (new)
	{
		temp = ft_env_substr(a, loc + 1, ft_strlen(a) - loc - 1, td->gc);
		if (!temp)
			return (error_general("malloc: env not initiated\n"));
		new->val = ft_env_strjoin(new->val, temp, td->gc);
	}
	else
		add_env_var(top_env, a, td->gc);
	return (0);
}

int	export_helper(t_env *top_env, char *arg, t_token_data *td)
{
	int		loc;

	if (is_valid(arg) < 0)
		return (error_general("export: not a valid identifier\n"));
	loc = find_sign(arg, '=');
	if (loc < 0)
		return (export_no_equals(top_env, arg, td));
	if (loc == 0 || loc == (int)ft_strlen(arg))
		return (error_general("export: not a valid identifier\n"));
	if (arg[loc - 1] == '+')
		return (export_append_mode(top_env, arg, loc, td));
	return (add_env_var(top_env, arg, td->gc));
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
