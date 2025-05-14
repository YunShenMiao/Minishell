/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:21:31 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/14 17:51:02 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_env(char **args, t_env *top_env)
{
	t_env	*temp;

	if (args[1])
		return (error_general("env: No such file or directory\n"));
	temp = top_env;
	while (temp->next && temp->val)
	{
		printf("%s=%s\n", temp->name, temp->val);
		temp = temp->next;
	}
	if (temp->val)
		printf("%s=%s\n", temp->name, temp->val);
	return (0);
}
