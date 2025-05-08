/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 14:55:26 by jwardeng          #+#    #+#             */
/*   Updated: 2025/05/08 15:51:28 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// creates ast nodes and setting its variables + linking them to
// left and right node -> aststructure
t_ast	*create_ast_node(t_token_data **token_data, t_tok_type type)
{
	t_ast	*new_node;

	new_node = (t_ast *)gc_malloc((*token_data)->gc, PARSING, sizeof(t_ast));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	new_node->args = NULL;
	new_node->file_name = NULL;
	new_node->quote = 0;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->cmd_path = NULL;
	new_node->HD = 0;
	new_node->gc = (*token_data)->gc;
	return (new_node);
}

// checks for current quote status & when encountering a quote
// either sets it to "in quote (1)" or "outside quote (0)"")
int	quote_status(t_token_data **token_data, char input)
{
	if (input == '\'' && ((*token_data)->in_SQ) == 0
		&& ((*token_data)->in_DQ) == 0)
		(*token_data)->in_SQ = 1;
	else if (input == '\"' && ((*token_data)->in_SQ) == 0
		&& ((*token_data)->in_DQ) == 0)
		(*token_data)->in_DQ = 1;
	else if (input == '\'' && (*token_data)->in_SQ == 1
		&& ((*token_data)->in_DQ) == 0)
		(*token_data)->in_SQ = 0;
	else if (input == '\"' && ((*token_data)->in_SQ) == 0
		&& ((*token_data)->in_DQ) == 1)
		(*token_data)->in_DQ = 0;
	if ((*token_data)->in_DQ == 1 || (*token_data)->in_SQ == 1)
		return (1);
	else
		return (0);
}
