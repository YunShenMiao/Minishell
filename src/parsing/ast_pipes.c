/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_pipes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 14:54:32 by jwardeng          #+#    #+#             */
/*   Updated: 2025/05/16 13:21:52 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// creates AST, root node if(pipe) is pipe,
// if (multiple pipes) last pipe
// if no pipe redir (with cmd + file/delim)
// if no redir -> just cmd with argos
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
