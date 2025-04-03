/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 14:14:16 by xueyang           #+#    #+#             */
/*   Updated: 2025/04/03 16:56:13 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_quote(t_token *print, t_env *top_env)
{
	char	*content;
	char	*name;
	int		i;
	t_env	*node;

	if (print->type == TOK_WORD_SQ)
	{
		if (ft_put_to_fd(print->value, 1) == 1)
			return (1);
	}
	if (print->type == TOK_WORD_DQ || print->type == TOK_WORD_NQ)
	{
		content = print->value;
		while(*content)
		{
			if (*content == '$')
			{
				i = 0;
				content++;
				while (*content != ' ')
				{
					name[i] = *content;
					content++;
					i++;
				}
				node = search_name_node(&top_env, name);
				if (ft_put_to_fd(node->val, 1) == 1)
					return (1);
			}
			else
			{
				if (write(1, content, 1) == -1)
					return (1);
			}
		}
	}
	return (0);
}

int	ft_echo(t_token *current, t_env *top_env)
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
		handle_quote(print, top_env);
		if (print->next && (print->next->type == TOK_WORD_DQ || print->next->type == TOK_WORD_SQ || print->next->type == TOK_WORD_NQ))
			if (ft_put_to_fd(" ", 1) == 1)
				return (1);
	}
	if (no_newline == 0)
		if (ft_put_to_fd("\n", 1) == 1)
			return (1);
	return (0);
}
