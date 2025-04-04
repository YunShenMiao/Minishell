/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 14:10:01 by jwardeng          #+#    #+#             */
/*   Updated: 2025/04/02 13:17:30 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// rn << >> < > at beginning is throwing error, in bash its not... should i handle?
// issue with left or right, especially redirections
// conflicting word-command logic in lexing and parsing...

// function is creating ast out of tokens starting with recursive calls 
// on lowest precedence (pipes)
t_ast	*build_ast(t_token_data **token_data)
{
	t_ast	*ast;
	t_token	*current;

	current = (*token_data)->token_list;
	ast = parse_pipes(token_data, &current);
	(*token_data)->ast = ast;
	if ((*token_data)->syntax_error == 1)
		return (NULL);
		printf("error: %d\n", (*token_data)->syntax_error);
	return (ast);
}
