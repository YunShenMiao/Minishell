/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 14:55:26 by jwardeng          #+#    #+#             */
/*   Updated: 2025/03/30 14:40:18 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


// // need to use perror
void	ft_perror_parsing(int error_id, char *error_info)
{
	if (error_id == INVALID_COMMAND)
		printf("🐢 minishell: '%s': command not found\n", error_info);
	if (error_id == SYNTAX_ERROR)
		printf("🐢 minishell: syntax error near unexpected token '%s'\n",
			error_info);
	if (error_id == UNCLOSED_QUOTES)
		printf("🐢 minishell: syntax error '%s'\n", error_info);
}

t_ast *create_ast_node(t_token_data **token_data, t_tok_type type)
{
    t_ast *new_node;
    
    new_node = (t_ast*)gc_malloc((*token_data)->gc, PARSING, sizeof(t_ast));
    if (!new_node)
    return(NULL);
    new_node->type = type;
    new_node->args = NULL;
    new_node->fd = -1;
    new_node->file_name = NULL;
    new_node->left = NULL;
    new_node->right = NULL;
    return(new_node);
}

// modified strndup to use gc_malloc & have starting and end position for allocating 
char *ft_strndup(t_gc *gc, const char *src, int start, int end)
{
	char	*address;
	char	*old_dest;

	if (end - start <= 0)
	return(NULL);
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

int	is_word(t_tok_type type)
{
	return (type == TOK_WORD_DQ || type == TOK_WORD_SQ || type == TOK_WORD_NQ
		|| type == TOK_EMPTY_WORD);
}
