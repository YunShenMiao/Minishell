/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_redirect.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 14:41:12 by jwardeng          #+#    #+#             */
/*   Updated: 2025/04/04 12:42:35 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_ast	*parse_append(t_token_data **token_data, t_token **current)
{
	t_ast	*re_node;

	if (!(*current) || (*current)->type != TOK_APPEND)
		return (NULL);
	*current = (*current)->next;
	if (!(*current) || !(is_word((*current)->type)))
	{
		if ((*token_data)->syntax_error == 0)
			ft_perror_parsing(SYNTAX_ERROR, (*current)->value);
		return ((*token_data)->syntax_error = 1, NULL);
	}
	re_node = create_ast_node(token_data, TOK_APPEND);
	re_node->file_name = ft_strndup((*token_data)->gc, (*current)->value, 0,
			ft_strlen((*current)->value));
	re_node->type = TOK_APPEND;
	re_node->fd = 1;
	*current = (*current)->next;
	return (re_node);
}

t_ast	*parse_heredoc(t_token_data **token_data, t_token **current)
{
	t_ast	*re_node;

	if (!(*current) || (*current)->type != TOK_HEREDOC)
		return (NULL);
	*current = (*current)->next;
	if (!(*current) || !(is_word((*current)->type)))
	{
		if ((*token_data)->syntax_error == 0)
			ft_perror_parsing(SYNTAX_ERROR, (*current)->value);
		return ((*token_data)->syntax_error = 1, NULL);
	}
	re_node = create_ast_node(token_data, TOK_HEREDOC);
	re_node->file_name = ft_strndup((*token_data)->gc, (*current)->value, 0,
			ft_strlen((*current)->value));
	re_node->type = TOK_HEREDOC;
	re_node->fd = 0;
	*current = (*current)->next;
	return (re_node);
}

t_ast	*parse_re_out(t_token_data **token_data, t_token **current)
{
	t_ast	*re_node;

	if (!(*current) || (*current)->type != TOK_REDIRECT_OUT)
		return (NULL);
	*current = (*current)->next;
	if (!(*current) || !(is_word((*current)->type)))
	{
		if ((*token_data)->syntax_error == 0)
			ft_perror_parsing(SYNTAX_ERROR, (*current)->value);
		return ((*token_data)->syntax_error = 1, NULL);
	}
	re_node = create_ast_node(token_data, TOK_REDIRECT_OUT);
	re_node->file_name = ft_strndup((*token_data)->gc, (*current)->value, 0,
			ft_strlen((*current)->value));
	re_node->type = TOK_REDIRECT_OUT;
	re_node->fd = 1;
	*current = (*current)->next;
	return (re_node);
}

t_ast	*parse_re_in(t_token_data **token_data, t_token **current)
{
	t_ast	*re_node;

	if (!(*current) || (*current)->type != TOK_REDIRECT_IN)
		return (NULL);
	*current = (*current)->next;
	if (!(*current) || !(is_word((*current)->type)))
	{
		if ((*token_data)->syntax_error == 0)
			ft_perror_parsing(SYNTAX_ERROR, (*current)->value);
		return ((*token_data)->syntax_error = 1, NULL);
	}
	re_node = create_ast_node(token_data, TOK_REDIRECT_IN);
	re_node->file_name = ft_strndup((*token_data)->gc, (*current)->value, 0,
			ft_strlen((*current)->value));
	re_node->type = TOK_REDIRECT_IN;
	re_node->fd = 0;
	*current = (*current)->next;
	return (re_node);
}

t_ast	*parse_redirections(t_token_data **token_data, t_token **current)
{
	if ((*current)->type == TOK_REDIRECT_IN)
		return (parse_re_in(token_data, current));
	else if ((*current)->type == TOK_REDIRECT_OUT)
		return (parse_re_out(token_data, current));
	else if ((*current)->type == TOK_APPEND)
		return (parse_append(token_data, current));
	else if ((*current)->type == TOK_HEREDOC)
		return (parse_heredoc(token_data, current));
	else
		return (NULL);
}
