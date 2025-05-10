/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 14:55:26 by jwardeng          #+#    #+#             */
/*   Updated: 2025/05/10 12:20:13 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// creates ast nodes and setting its variables + linking them to
// left and right node
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
	new_node->hd = 0;
	new_node->gc = (*token_data)->gc;
	return (new_node);
}

// checks for current quote status & when encountering a quote
// either sets it to "in quote (1)" or "outside quote (0)"")
int	quote_status(t_token_data **token_data, char input)
{
	if (input == '\'' && ((*token_data)->sq) == 0
		&& ((*token_data)->dq) == 0)
		(*token_data)->sq = 1;
	else if (input == '\"' && ((*token_data)->sq) == 0
		&& ((*token_data)->dq) == 0)
		(*token_data)->dq = 1;
	else if (input == '\'' && (*token_data)->sq == 1
		&& ((*token_data)->dq) == 0)
		(*token_data)->sq = 0;
	else if (input == '\"' && ((*token_data)->sq) == 0
		&& ((*token_data)->dq) == 1)
		(*token_data)->dq = 0;
	if ((*token_data)->dq == 1 || (*token_data)->sq == 1)
		return (1);
	else
		return (0);
}

// checks if cmd is builtin
int	token_command(char *value)
{
	if (ft_ministrcmp(value, "cd") == 0)
		return (0);
	else if (ft_ministrcmp(value, "echo") == 0)
		return (0);
	else if (ft_ministrcmp(value, "pwd") == 0)
		return (0);
	else if (ft_ministrcmp(value, "env") == 0)
		return (0);
	else if (ft_ministrcmp(value, "export") == 0)
		return (0);
	if (ft_ministrcmp(value, "unset") == 0)
		return (0);
	else if (ft_ministrcmp(value, "exit") == 0)
		return (0);
	else
		return (1);
}

// checks if args[0] is a valid cmd or builtin. Checks cmd by checking
// for valid path and also checking that the path is not a directory
int	valid_cmd(t_token_data **token_data, t_ast *node)
{
	DIR	*dir;

	dir = NULL;
	node->cmd_path = find_path(node->args[0], (*token_data)->envp,
			(*token_data)->gc);
	if (node->cmd_path == NULL && token_command(node->args[0]) == 1)
	{
		if ((*token_data)->syntax_error == 0)
			ft_perror_parsing(token_data, INVALID_COMMAND, node->args[0]);
		(*token_data)->syntax_error = 1;
		return (1);
	}
	if (node->args[0][0] == '/')
		dir = opendir(node->cmd_path);
	if (dir)
	{
		closedir(dir);
		if ((*token_data)->syntax_error == 0)
			ft_perror_parsing(token_data, IS_DIR, node->args[0]);
		(*token_data)->syntax_error = 1;
		return (1);
	}
	return (0);
}
