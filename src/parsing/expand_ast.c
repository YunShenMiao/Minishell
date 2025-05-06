/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_ast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:29:30 by jwardeng          #+#    #+#             */
/*   Updated: 2025/05/06 18:53:12 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// new logic for handling quotes and environment variables
// NEED TO: check memory leaks
// NEED TO: handle error returns throughout functions
// NEED TO: norminette & generally structure and name more
// logically (cmd | quotes | env var)
// NEED TO: go to token_list and norminette tokenize (+ check efficiency)
// NEED TO: go through structs and delete unused vars
// proj-checks: ms1 (safety copy before changes 10.04)
// NEED TO: maybe combine redirect functions in ast_redirect.c
// handle ~ ? 

// checks if cmd is builtin
int	token_command(char *value, size_t len)
{
	if (ft_strncmp(value, "cd", len) == 0)
		return (0);
	else if (ft_strncmp(value, "echo", len) == 0)
		return (0);
	else if (ft_strncmp(value, "echo-n", len) == 0)
		return (0);
	else if (ft_strncmp(value, "pwd", len) == 0)
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

// checks if args[0] is a valid cmd or builtin, returns error if not
int	valid_cmd(t_token_data **token_data, t_ast *node)
{
	DIR	*dir;

	dir = NULL;
	node->cmd_path = find_path(node->args[0], (*token_data)->envp,
			(*token_data)->gc);
	if (node->cmd_path == NULL && token_command(node->args[0],
			ft_strlen(node->args[0])) == 1)
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

void	handle_env_cmd(char **new, char **rest, t_gc *gc)
{
	int	count;
	char *str;

	str = *new;
	count = 0;
	while (str && str[count] != ' ')
		count++;
	(*rest) = ft_env_substr(str, count + 1, ft_strlen(str), gc);
	str[count] = '\0';
}

// passes each arg of the command to handle the quotes & checks if arg[0]
// is a valid command or not
// need to allocate new 2dstring to handle env avr expansion
int	command_args(t_ast *node, int *i, t_token_data **token_data)
{
	char	**args;
	char	*new;
	char	*rest;
	int		count;

	args = gc_malloc((*token_data)->gc, PARSING, 100 * sizeof(char *));
	rest = NULL;
	count = 0;
	while (node->args[*i] != NULL)
	{
		new = handle_quotes(token_data, &node->args[*i]);
		if (new == NULL)
			return (1);
		if ((*token_data)->env_cmd == 1 && (*i) == 0)
		{
			handle_env_cmd(&new, &rest, ((*token_data)->gc));
			args[count] = new;
			count++;
			if (rest)
			args[count] = rest;
		}
		else
			args[count] = new;
		(*i)++;
		count++;
	}
	args[count] = NULL;
	node->args = args;
	if (valid_cmd(token_data, node) == 1)
		return (1);
	return (0);
}

// traverses through the ast and handles quotes and
// environment var expansion for cmd-args & file-names in quotes_env.c
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
	else if (node->type == TOK_FILE)
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
