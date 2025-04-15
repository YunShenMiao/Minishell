/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 18:10:25 by xueyang           #+#    #+#             */
/*   Updated: 2025/04/15 22:18:05 by xueyang          ###   ########.fr       */
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

// is there anything as gc_free? check how to free here again, or check if a var= NULL before freeing it in the gc_free_all
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
	free(to_del->name);
	free(to_del->val);
	free(to_del);
}

t_env	*search_name_node(t_env *top, char *name)
{
	t_env	*temp;

	temp = top;
	while (temp)
	{
		if (ft_strlen(temp->name) == ft_strlen(name) && \
		ft_strncmp(temp->name, name, ft_strlen(name)) == 0)
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
		if (ft_strlen(temp->name) == ft_strlen(name) && \
		ft_strncmp(temp->name, name, ft_strlen(name)) == 0)
			return (temp->val);
		temp = temp->next;
	}
	return (NULL);
}

int	update_env_var(t_env **lst, char *name, char *new_val, t_gc *gc)
{
	t_env	*temp;
	char	*old_val;

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
		old_val = temp->val;
		temp->val = new_val; // or ft_strdup(new_val) ...?
		free(old_val);
	}
	return (0);
}
