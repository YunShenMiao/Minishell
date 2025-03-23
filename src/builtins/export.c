/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:21:43 by xueyang           #+#    #+#             */
/*   Updated: 2025/03/23 21:19:09 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_export(t_env	**top_env)
{

}

int	ft_export(t_env	**top_env, t_token *current)
{
	char	*assign;

	if (!current->next)
		print_export(top_env);
	else if (current->next->next)
	{
		perror("export: bad assign");
		return (1);
	}
	else
	{
		assign = current->next;
		
	}
}