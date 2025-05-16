/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_ast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:29:30 by jwardeng          #+#    #+#             */
/*   Updated: 2025/05/16 17:07:29 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_env_cmd(char **new, char **rest, t_gc *gc)
{
	int		count;
	char	*str;

	str = *new;
	count = 0;
	while (str && str[count] != ' ')
		count++;
	(*rest) = ft_env_substr(str, count + 1, ft_strlen(str), gc);
	str[count] = '\0';
}

int	bi_echo(t_ast *node, int *i)
{
	int	count;
	int	count2;

	count = *i;
	while (node->args[count] != NULL)
	{
		if (node->args[count][0] != '-')
			break ;
		count2 = 1;
		if (node->args[count][count2] == '\0')
			break ;
		while (node->args[count][count2] == 'n')
			count2++;
		if (node->args[count][count2] != '\0')
			break ;
		count++;
	}
	if (count > *i)
	{
		*i = count;
		return (2);
	}
	*i = count;
	return (0);
}

// here could implement wildcard check and then realloc
int	command_args_loop(t_ast *node, int *i,
	t_token_data **token_data, char **args)
{
	char	*new;
	char	*rest;
	int		count;

	count = 0;
	rest = NULL;
	while (node->args[*i] != NULL)
	{
		new = handle_quotes(token_data, &node->args[*i]);
		if (new == NULL)
			return (1);
		if ((*token_data)->env_cmd == 1 && (*i) == 0)
		{
			handle_env_cmd(&new, &rest, (*token_data)->gc);
			args[count++] = new;
			if (rest)
				args[count++] = rest;
		}
		else
			args[count++] = new;
		(*i)++;
	}
	args[count] = NULL;
	return (0);
}

// passes each arg of the command to handle the quotes & checks if arg[0]
// is a valid command or not
// need to allocate new 2dstring to handle env avr expansion
int	command_args(t_ast *node, int *i, t_token_data **td)
{
	char	**args;
	int		count;

	args = (char **)gc_malloc((*td)->gc, PARSING, 1000 * sizeof(char *));
	count = 0;
	if (node->args[*i] != NULL && ft_ministrcmp(node->args[*i], "echo") == 0)
	{
		args[count] = handle_quotes(td, &node->args[*i]);
		if (args[count] == NULL)
			return (1);
		count++;
		if (*i == 0 && ft_ministrcmp(node->args[*i], "echo") == 0)
		{
			(*i)++;
			if (bi_echo(node, i) == 2)
			{
				args[count] = ft_env_strdup("-n", (*td)->gc, PARSING);
				count++;
			}
		}
	}
	if (command_args_loop(node, i, td, args + count) == 1)
		return (1);
	return (node->args = args, 0);
}

// traverses through the ast and handles quotes and
// environment var expansion for cmd-args & file-names in quotes_env.c
int	expand_ast_nodes(t_token_data **td, t_ast **ast)
{
	t_ast	*node;
	int		i;

	node = *ast;
	i = 0;
	if (node->type == TOK_COMMAND)
	{
		if (command_args(node, &i, td) == 1 || valid_cmd(td, node) == 1)
			return (1);
	}
	else if (node->type == TOK_FILE)
	{
		if (node->file_name[0] == '\'' || node->file_name[0] == '\"')
			node->quote = 1;
		if (node->hd == 1)
			(*td)->hd = 1;
		node->file_name = handle_quotes(td, &node->file_name);
		if (node->file_name == NULL)
			return (1);
	}
	if (node->left != NULL)
		expand_ast_nodes(td, &node->left);
	if (node->right != NULL)
		expand_ast_nodes(td, &node->right);
	return (0);
}
