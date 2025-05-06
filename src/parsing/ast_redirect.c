/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_redirect.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 14:41:12 by jwardeng          #+#    #+#             */
/*   Updated: 2025/05/06 18:53:33 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
	re_node->type = TOK_APPEND;
	file_node = create_ast_node(token_data, TOK_FILE);
	file_node->file_name = ft_strndup((*token_data)->gc, (*current)->value, 0,
			ft_strlen((*current)->value));
	re_node->right = file_node;
	*current = (*current)->next;
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
	re_node->type = TOK_HEREDOC;
	file_node = create_ast_node(token_data, TOK_FILE);
	file_node->file_name = ft_strndup((*token_data)->gc, (*current)->value, 0,
			ft_strlen((*current)->value));
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
	re_node->type = TOK_REDIRECT_OUT;
	file_node = create_ast_node(token_data, TOK_FILE);
	file_node->file_name = ft_strndup((*token_data)->gc, (*current)->value, 0,
			ft_strlen((*current)->value));
	re_node->right = file_node;
	*current = (*current)->next;
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
	re_node->type = TOK_REDIRECT_IN;
	file_node = create_ast_node(token_data, TOK_FILE);
	file_node->file_name = ft_strndup((*token_data)->gc, (*current)->value, 0,
			ft_strlen((*current)->value));
	re_node->left = file_node;
	*current = (*current)->next;
	// if ((*current)->type == TOK_WORD)
	// re_node->right = create_ast_node(token_data, TOK_COMMAND);
	// else
	re_node->right = prev;
	return (re_node);
}

t_ast	*parse_redirections(t_token_data **token_data, t_token **current,
		t_ast *prev)
{
	t_ast	*redir;
	// t_ast *cmd;
	redir = NULL;
	// cmd = NULL;
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
		// else if ((*current)->type == TOK_WORD)
		// {
		// 	cmd = create_ast_node(token_data, TOK_COMMAND);
		// 	parse_command_args(token_data, current,cmd);
		// 	redir->left = cmd;
		// }
		prev = redir;
	}
	return (redir);
}
