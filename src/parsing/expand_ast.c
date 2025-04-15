/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_ast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:29:30 by jwardeng          #+#    #+#             */
/*   Updated: 2025/04/15 13:54:06 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// new logic for handling quotes and environment variables
// NEED TO: check memory leaks
// NEED TO: create expand env logic
// NEED TO: handle error returns throughout functions
// NEED TO: norminette & generally structure and name more logically (cmd | quotes | env var)
// NEED TO: go to token_list and norminette tokenize (+ check efficiency)
// NEED TO: go through structs and delete unused vars
// proj-checks: ms1 (safety copy before changes 10.04)
// NEED TO: maybe combine redirect functions in ast_redirect.c
// a=b echo $a rn is giving invalid command error -> should be working?
// --> handling it by creating temp_var array; maybe split logic to first check for temp var and then when creating the args array
// having different counts for prev args[i] and new args[i], e.g. temp_var[i]
// -> max i = 3
// args[0] -> args[3++] ->newargs[0++]

int	token_command(char *value, size_t len)
{
	if (ft_strncmp(value, "cd", len) == 0 || ft_strncmp(value, "\"cd\"",
			len) == 0 || ft_strncmp(value, "\'cd\'", len) == 0)
		return (0);
	else if (ft_strncmp(value, "echo", len) == 0 || ft_strncmp(value,
			"\"echo\"", len) == 0 || ft_strncmp(value, "\'echo\'", len) == 0)
		return (0);
	else if (ft_strncmp(value, "echo-n", len) == 0 || ft_strncmp(value,
			"\"echo-n\"", len) == 0 || ft_strncmp(value, "\'echo-n\'",
			len) == 0)
		return (0);
	else if (ft_strncmp(value, "pwd", len) == 0 || ft_strncmp(value, "\"pwd\"",
			len) == 0 || ft_strncmp(value, "\'pwd\'", len) == 0)
		return (0);
	else if (ft_strncmp(value, "env", len) == 0)
		return (0);
	else if (ft_strncmp(value, "export", len) == 0)
		return (0);
	if (ft_strncmp(value, "unset", len) == 0)
		return (0);
	else if (ft_strncmp(value, "exit", len) == 0)
		return (0);
	else
		return (1);
}

int	valid_cmd(t_token_data **token_data, t_ast *node)
{
	node->cmd_path = find_path(node->args[0], (*token_data)->envp);
	if (node->cmd_path == NULL && token_command(node->args[1],
			ft_strlen(node->args[1])) == 1)
	{
		if ((*token_data)->syntax_error == 0)
			ft_perror_parsing(INVALID_COMMAND, node->args[0]);
		(*token_data)->syntax_error = 1;
		return (1);
	}
	return (0);
}

int	command_args(t_ast *node, int *i, t_token_data **token_data)
{
	while (node->args[*i] != NULL)
	{
		node->args[*i] = handle_quotes(token_data, &node->args[*i]);
		if (node->args[*i] == NULL)
			return (1);
		(*i)++;
	}
	if (valid_cmd(token_data, node) == 1)
		return (1);
	return (0);
}

int	expand_ast_nodes(t_token_data **token_data, t_ast **ast)
{
	t_ast	*node;
	int		i;

	node = *ast;
	i = 0;
	if (node->type == TOK_COMMAND)
	{
		if (command_args(node, &i, token_data) == 1)
			return (1);
	}
	else if (node->type == TOK_APPEND || node->type == TOK_HEREDOC
		|| node->type == TOK_REDIRECT_IN || node->type == TOK_REDIRECT_OUT)
	{
		node->file_name = handle_quotes(token_data, &node->file_name);
		if (node->file_name == NULL)
			return (1);
	}
	if (node->left != NULL)
		expand_ast_nodes(token_data, &node->left);
	if (node->right != NULL)
		expand_ast_nodes(token_data, &node->right);
	return (0);
}
