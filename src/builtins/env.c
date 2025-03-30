/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:21:31 by xueyang           #+#    #+#             */
/*   Updated: 2025/03/30 15:47:37 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_env(t_env *top_env)
{
	t_env	*temp;

	temp = top_env;
	while (temp->next && temp->val)
	{
		printf("%s=%s\n", temp->name, temp->val);
		temp = temp->next;
	}
	printf("%s=%s\n", temp->name, temp->val);
	return (0);
}
