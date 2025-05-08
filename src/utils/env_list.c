/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 18:10:25 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/08 17:09:49 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_env_add_back(t_env **lst, t_env *new)
{
	t_env	*last_node;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
		*lst = new;
	else
	{
		last_node = ft_env_last(*lst);
		last_node->next = new;
	}
}

void	ft_env_del(t_env **top, t_env *to_del)
{
	t_env	*before;

	if (*top == to_del)
	{
		*top = to_del->next;
	}
	else
	{
		before = *top;
		while (before && before->next != to_del)
			before = before->next;
		if (before)
			before->next = to_del->next;
	}
}

t_env	*search_name_node(t_env *top, char *name)
{
	t_env	*temp;

	temp = top;
	while (temp)
	{
		if (ft_strlen(temp->name) == ft_strlen(name) && ft_strncmp(temp->name,
				name, ft_strlen(name)) == 0)
			return (temp);
		temp = temp->next;
	}
	return (NULL);
}

char	*search_name_val(t_env *top, char *name)
{
	t_env	*temp;

	temp = top;
	while (temp)
	{
		if (ft_strlen(temp->name) == ft_strlen(name) && ft_strncmp(temp->name,
				name, ft_strlen(name)) == 0)
			return (temp->val);
		temp = temp->next;
	}
	return (NULL);
}

int	update_env_var(t_env **lst, char *name, char *new_val, t_gc *gc)
{
	t_env	*temp;

	if (!search_name_node(*lst, name))
	{
		temp = create_env(name, new_val, gc);
		if (!temp)
			return (error_general("malloc: fail to create new env var"));
		ft_env_add_back(lst, temp);
	}
	else
	{
		temp = search_name_node(*lst, name);
		temp->val = new_val;
	}
	return (0);
}
