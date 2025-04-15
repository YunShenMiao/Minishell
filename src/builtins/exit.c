/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:21:25 by xueyang           #+#    #+#             */
/*   Updated: 2025/04/15 18:21:30 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// int	ft_exit(t_token *current, t_gc *gc)
// {
// 	if (current->type == TOK_COMMAND)
// 	{
// 		gc_free_all(gc);
// 		exit(EXIT_SUCCESS);
// 	}
// }

int	ft_exit(t_gc *gc)
{
	gc_free_all(gc);
	exit(EXIT_SUCCESS);
}
