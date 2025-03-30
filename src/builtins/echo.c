/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 14:14:16 by xueyang           #+#    #+#             */
/*   Updated: 2025/03/30 15:35:33 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//need to consider env variables
int	ft_echo(t_token *current)
{
	int		no_newline;
	t_token	*print;

	if (ft_strncmp(current->value, "echo", 4) == 0)
		no_newline = 0;
	if (ft_strncmp(current->value, "echo-n", 6) == 0)
		no_newline = 1;
	print = current;
	while (print->next && (print->next->type == TOK_WORD_DQ || print->next->type == TOK_WORD_SQ || print->next->type == TOK_WORD_NQ))
	{
		print = print->next;
		if (ft_put_to_fd(current->next, 1) == 1)
			return (1);
		if (print->next && (print->next->type == TOK_WORD_DQ || print->next->type == TOK_WORD_SQ || print->next->type == TOK_WORD_NQ))
			if (ft_put_to_fd(" ", 1) == 1)
				return (1);
	}
	if (no_newline == 0)
		if (ft_put_to_fd("\n", 1) == 1)
			return (1);
	return (0);
}
