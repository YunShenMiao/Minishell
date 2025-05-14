/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 14:10:01 by jwardeng          #+#    #+#             */
/*   Updated: 2025/05/14 18:51:33 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
	return (ast);
}
