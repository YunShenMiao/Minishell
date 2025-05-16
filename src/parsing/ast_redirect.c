/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_redirect.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 14:41:12 by jwardeng          #+#    #+#             */
/*   Updated: 2025/05/16 13:22:58 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// for: parse_append, parse_heredoc, parse_re_in & parse_re_out
// checks if there is redirect, checks syntax (redirect needs file/delim)
// creates file node and left if re_in, right if re_out
// + prev -> cmd or other redirect
t_ast	*parse_append(t_token_data **token_data, t_token **current, t_ast *prev)
{
	t_ast	*re_node;
	t_ast	*file_node;

	if (!(*current) || (*current)->type != TOK_APPEND)
		return (NULL);
	*current = (*current)->next;
	if (!(*current) || (*current)->type != TOK_WORD)
	{
		if ((*token_data)->syntax_error == 0)
			ft_perror_parsing(token_data, SYNTAX_ERROR, (*current)->value);
		return ((*token_data)->syntax_error = 1, NULL);
	}
	re_node = create_ast_node(token_data, TOK_APPEND);
	file_node = create_ast_node(token_data, TOK_FILE);
	file_node->file_name = ft_strndup((*token_data)->gc, (*current)->value, 0,
			ft_strlen((*current)->value));
	re_node->right = file_node;
	*current = (*current)->next;
	if (!prev)
		re_node->left = add_cmd(*token_data, TOK_APPEND);
	else
		re_node->left = prev;
	return (re_node);
}

t_ast	*parse_heredoc(t_token_data **token_data, t_token **current,
		t_ast *prev)
{
	t_ast	*re_node;
	t_ast	*file_node;

	if (!(*current) || (*current)->type != TOK_HEREDOC)
		return (NULL);
	*current = (*current)->next;
	if (!(*current) || (*current)->type != TOK_WORD)
	{
		if ((*token_data)->syntax_error == 0)
			ft_perror_parsing(token_data, SYNTAX_ERROR, (*current)->value);
		return ((*token_data)->syntax_error = 1, NULL);
	}
	re_node = create_ast_node(token_data, TOK_HEREDOC);
	file_node = create_ast_node(token_data, TOK_FILE);
	file_node->file_name = ft_strndup((*token_data)->gc, (*current)->value, 0,
			ft_strlen((*current)->value));
	file_node->hd = 1;
	re_node->left = file_node;
	*current = (*current)->next;
	re_node->right = prev;
	return (re_node);
}

t_ast	*parse_re_out(t_token_data **token_data, t_token **current, t_ast *prev)
{
	t_ast	*re_node;
	t_ast	*file_node;

	if (!(*current) || (*current)->type != TOK_REDIRECT_OUT)
		return (NULL);
	*current = (*current)->next;
	if (!(*current) || (*current)->type != TOK_WORD)
	{
		if ((*token_data)->syntax_error == 0)
			ft_perror_parsing(token_data, SYNTAX_ERROR, (*current)->value);
		return ((*token_data)->syntax_error = 1, NULL);
	}
	re_node = create_ast_node(token_data, TOK_REDIRECT_OUT);
	file_node = create_ast_node(token_data, TOK_FILE);
	file_node->file_name = ft_strndup((*token_data)->gc, (*current)->value, 0,
			ft_strlen((*current)->value));
	re_node->right = file_node;
	*current = (*current)->next;
	if (!prev)
		re_node->left = add_cmd(*token_data, TOK_REDIRECT_OUT);
	else
		re_node->left = prev;
	return (re_node);
}

t_ast	*parse_re_in(t_token_data **token_data, t_token **current, t_ast *prev)
{
	t_ast	*re_node;
	t_ast	*file_node;

	if (!(*current) || (*current)->type != TOK_REDIRECT_IN)
		return (NULL);
	*current = (*current)->next;
	if (!(*current) || (*current)->type != TOK_WORD)
	{
		if ((*token_data)->syntax_error == 0)
			ft_perror_parsing(token_data, SYNTAX_ERROR, (*current)->value);
		return ((*token_data)->syntax_error = 1, NULL);
	}
	re_node = create_ast_node(token_data, TOK_REDIRECT_IN);
	file_node = create_ast_node(token_data, TOK_FILE);
	file_node->file_name = ft_strndup((*token_data)->gc, (*current)->value, 0,
			ft_strlen((*current)->value));
	re_node->left = file_node;
	*current = (*current)->next;
	if (!prev)
		re_node->right = add_cmd(*token_data, TOK_REDIRECT_IN);
	else
		re_node->right = prev;
	return (re_node);
}

// prev either cmd node or other redirect (to handle echo miao > out >> out > out)
t_ast	*parse_redirections(t_token_data **token_data, t_token **current,
		t_ast *prev)
{
	t_ast	*redir;

	redir = NULL;
	while (*current && ((*current)->type == TOK_REDIRECT_IN
			|| (*current)->type == TOK_REDIRECT_OUT
			|| (*current)->type == TOK_APPEND
			|| (*current)->type == TOK_HEREDOC))
	{
		if ((*current)->type == TOK_REDIRECT_IN)
			redir = (parse_re_in(token_data, current, prev));
		else if ((*current)->type == TOK_REDIRECT_OUT)
			redir = (parse_re_out(token_data, current, prev));
		else if ((*current)->type == TOK_APPEND)
			redir = (parse_append(token_data, current, prev));
		else if ((*current)->type == TOK_HEREDOC)
			redir = (parse_heredoc(token_data, current, prev));
		prev = redir;
	}
	return (redir);
}
