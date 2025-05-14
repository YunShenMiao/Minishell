/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:21:36 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/14 16:50:16 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_valid_name(char *name)
{
	int	i;

	i = 0;
	if (*name != 95 && !(*name >= 65 && *name <= 90) && \
	!(*name >= 97 && *name <= 122))
		return (-1);
	if (name[0] == '-')
	{
		error_general("unset: usage: not supporting flags\n");
		exit(2);
	}
	while (name[i])
	{
		if (name[i] != 95 && !(name[i] >= 65 && name[i] <= 90) && \
		!(name[i] >= 97 && name[i] <= 122) && !(name[i] >= 48 && name[i] <= 57))
			return (-1);
		i++;
	}
	return (0);
}

void	unset_helper(char **args, t_env	*top_env, int *invalid_count)
{
	t_env	*to_del;
	int		i;

	i = 0;
	while (args[i + 1])
	{
		i++;
		if (is_valid_name(args[i]) == 0)
		{
			to_del = search_name_node(top_env, args[i]);
			if (to_del)
				ft_env_del(&top_env, to_del);
		}
		else
			(*invalid_count)++;
	}
}

int	ft_unset(t_env	*top_env, char **args)
{
	int	invalid_count;

	if (!args[1])
		return (0);
	else
	{
		invalid_count = 0;
		unset_helper(args, top_env, &invalid_count);
		if (invalid_count > 0)
			return (error_general("unset: not a valid identifier\n"));
	}
	return (0);
}
