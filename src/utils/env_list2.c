/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 18:10:25 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/08 17:08:34 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env	*create_env(char *name, char *value, t_gc *gc)
{
	t_env	*env_var;

	env_var = gc_malloc(gc, ENV, sizeof(t_env));
	if (!env_var)
		return (NULL);
	env_var->name = name;
	env_var->val = value;
	env_var->next = NULL;
	return (env_var);
}

t_env	*ft_env_last(t_env *lst)
{
	t_env	*temp;

	if (!lst)
		return (NULL);
	temp = lst;
	while (temp->next)
	{
		temp = temp->next;
	}
	return (temp);
}
