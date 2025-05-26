/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 14:50:00 by jwardeng          #+#    #+#             */
/*   Updated: 2025/05/26 15:39:05 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// handles (> out && < in by adding cat && echo -n as cmd)
// is called in ast_redirect.c
t_ast	*add_cmd(t_token_data *token_data, t_tok_type type)
{
	t_ast	*cmd_node;

	if (type == TOK_APPEND || type == TOK_REDIRECT_OUT)
	{
		cmd_node = create_ast_node(&token_data, TOK_COMMAND);
		cmd_node->args = (char **)gc_malloc(token_data->gc, PARSING,
				3 * sizeof(char *));
		cmd_node->args[0] = ft_env_strdup("echo", token_data->gc, PARSING);
		cmd_node->args[1] = ft_env_strdup("-n", token_data->gc, PARSING);
		cmd_node->args[2] = NULL;
	}
	else
	{
		cmd_node = create_ast_node(&token_data, TOK_COMMAND);
		cmd_node->args = (char **)gc_malloc(token_data->gc, PARSING,
				2 * sizeof(char *));
		cmd_node->args[0] = ft_env_strdup("cat", token_data->gc, PARSING);
		cmd_node->args[1] = NULL;
	}
	return (cmd_node);
}

// adds all word_tokens as cmd arguments, arg[0] is cmd
void	parse_command_args(t_token_data **token_data, t_token **current,
		t_ast *cmd_node)
{
	int	i;

	i = 0;
	while (*current && (*current)->type == TOK_WORD)
	{
		cmd_node->args[i] = ft_strndup((*token_data)->gc, (*current)->value, 0,
				ft_strlen((*current)->value));
		*current = (*current)->next;
		i++;
	}
	cmd_node->args[i] = NULL;
}

// if no cmd, call redirections (to handle '> out') 
// [couldve added cat/echo here instead]
// otherwise creates cmd_node with args and then creates re_node
// if we have a re_node we return the re_node 
// with cmd added to it in ast_redirect, else just cmd
// a little mixed up logically
t_ast	*parse_command(t_token_data **token_data, t_token **current)
{
	t_ast	*cmd_node;
	t_ast	*re_node;

	if (!(*current) || (*current)->type != TOK_WORD)
	{
		re_node = parse_redirections(token_data, current, NULL);
		if (re_node != NULL)
			return (re_node);
		else if ((*token_data)->syntax_error == 0)
			ft_perror_parsing(token_data, SYNTAX_ERROR, (*current)->value);
		return ((*token_data)->syntax_error = 1, NULL);
	}
	cmd_node = create_ast_node(token_data, TOK_COMMAND);
	cmd_node->args = (char **)gc_malloc((*token_data)->gc, PARSING,
			sizeof(char *) * 10000);
	parse_command_args(token_data, current, cmd_node);
	re_node = parse_redirections(token_data, current, cmd_node);
	if (re_node)
		return (re_node);
	return (cmd_node);
}
