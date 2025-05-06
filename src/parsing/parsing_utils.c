/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 14:55:26 by jwardeng          #+#    #+#             */
/*   Updated: 2025/05/06 18:54:03 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// // need to use perrors
void	ft_perror_parsing(t_token_data **token_data, int error_id,
		char *error_info)
{
	if (error_id == INVALID_COMMAND)
	{
		write(2, "🐢 minishell: ", 16);
		write(2, error_info, ft_strlen(error_info));
		write(2, ": command not found\n", 21);
		(*token_data)->last_exit = 127;
	}
	else if (error_id == SYNTAX_ERROR)
	{
		write(2, "🐢 minishell: syntax error near unexpected token '", 52);
		write(2, error_info, ft_strlen(error_info));
		write(2, "'\n", 2);
		(*token_data)->last_exit = 2;
	}
	else if (error_id == UNCLOSED_QUOTES)
	{
		write(2, "🐢 minishell: syntax error ", 29);
		write(2, error_info, ft_strlen(error_info));
		write(2, "\n", 2);
		(*token_data)->last_exit = 2;
	}
	else if (error_id == IS_DIR)
	{
		write(2, "🐢 minishell: ", 16);
		write(2, error_info, ft_strlen(error_info));
		write(2, ": is a directory\n", 17);
		(*token_data)->last_exit = 126;
	}
}

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
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->cmd_path = NULL;
	new_node->gc = (*token_data)->gc;
	return (new_node);
}

// modified strndup to use gc_malloc
// & have starting and end position for allocating
char	*ft_strndup(t_gc *gc, const char *src, int start, int end)
{
	char	*address;
	char	*old_dest;

	if (end - start < 0)
		return (NULL);
	address = (char *)gc_malloc(gc, TOKENS, end - start + 1);
	old_dest = address;
	if (address == NULL)
		return (NULL);
	while (src[start] != '\0' && start < end)
	{
		*address = src[start];
		address++;
		start++;
	}
	*address = '\0';
	return (old_dest);
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
