/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_redirect.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 14:41:12 by jwardeng          #+#    #+#             */
/*   Updated: 2025/05/07 18:53:20 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_ast *add_cmd(t_token_data *token_data, t_tok_type type)
{
	t_ast *cmd_node;

	if (type == TOK_APPEND || type == TOK_REDIRECT_OUT)
		{
		cmd_node = create_ast_node(&token_data, TOK_COMMAND);
		cmd_node->args = (char**)gc_malloc(token_data->gc, PARSING, sizeof(char**));
		cmd_node->args[0] = ft_env_strdup("echo", token_data->gc);
		cmd_node->args[1] = ft_env_strdup("-n", token_data->gc);
		cmd_node->args[2] = NULL;
		}
	else
		{
		cmd_node = create_ast_node(&token_data, TOK_COMMAND);
		cmd_node->args = (char**)gc_malloc(token_data->gc, PARSING, sizeof(char**));
		cmd_node->args[0] = ft_env_strdup("cat", token_data->gc);
		cmd_node->args[1] = NULL;
		}
	return(cmd_node);
}

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
	file_node->HD = 1;
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
