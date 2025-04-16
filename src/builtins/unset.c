/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:21:36 by xueyang           #+#    #+#             */
/*   Updated: 2025/04/15 22:18:07 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_valid_name(char *name)
{
	if (*name != 95 && !(*name >= 65 && *name <= 90) && \
	 !(*name >= 97 && *name <= 122))
		return (-1);
	while(*name)
	{
		if (*name != 95 && !(*name >= 65 && *name <= 90) && \
		!(*name >= 97 && *name <= 122) && !(*name >= 48 && *name <= 57))
			return (-1);
		name++;
	}
	return (0);
}

// int	ft_unset(t_env	*top_env, t_token *current)
// {
// 	t_env	*to_del;
// 	int		invalid_count;


// 	if (!current->next)
// 		return (0);
// 	else
// 	{
// 		invalid_count = 0;
// 		while (current->next)
// 		{
// 			current = current->next;
// 			if (is_valid_name(current->value) == 0)
// 			{
// 				to_del = search_name_node(&top_env, current->value);
// 				if (to_del)
// 					ft_env_del(top_env, to_del);
// 			}
// 			else
// 				invalid_count++;
// 		}
// 		if (invalid_count > 0)
// 			return (error_general("unset: not a valid identifier"));
// 	}
// 	return (0);
// }

int	ft_unset(t_env	*top_env, char **args)
{
	t_env	*to_del;
	int		invalid_count;
	int		i;


	if (!args[1])
		return (0);
	else
	{
		i = 0;
		invalid_count = 0;
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
				invalid_count++;
		}
		if (invalid_count > 0)
			return (error_general("unset: not a valid identifier"));
	}
	return (0);
}