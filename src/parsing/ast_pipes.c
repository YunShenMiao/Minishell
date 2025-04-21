/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_pipes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 14:54:32 by jwardeng          #+#    #+#             */
/*   Updated: 2025/04/21 16:56:07 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_ast	*parse_pipes(t_token_data **token_data, t_token **current)
{
	t_ast	*right;
	t_ast	*left;
	t_ast	*pipe;

	left = parse_command(token_data, current);
	if (!left)
		return ((*token_data)->syntax_error = 1, NULL);
	while (*current && (*current)->type == TOK_PIPE)
	{
		*current = (*current)->next;
		right = parse_command(token_data, current);
		if (!right)
		{
			if ((*token_data)->syntax_error == 0)
				ft_perror_parsing(token_data, SYNTAX_ERROR, (*current)->value);
			return ((*token_data)->syntax_error = 1, NULL);
		}
		pipe = create_ast_node(token_data, TOK_PIPE);
		pipe->left = left;
		pipe->right = right;
		left = pipe;
	}
	return (left);
}
