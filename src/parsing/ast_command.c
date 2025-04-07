/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 14:50:00 by jwardeng          #+#    #+#             */
/*   Updated: 2025/04/07 15:53:03 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	parse_command_args(t_token_data **token_data, t_token **current,
		t_ast *cmd_node)
{
	int	i;

	i = 0;
	while (*current && (is_word((*current)->type)
			|| (*current)->type == TOK_COMMAND))
	{
		cmd_node->args[i] = ft_strndup((*token_data)->gc, (*current)->value, 0,
				ft_strlen((*current)->value));
		*current = (*current)->next;
		i++;
	}
	cmd_node->cmd_path = find_path(cmd_node->args[0], (*token_data)->envp);
	if (cmd_node->cmd_path == NULL && token_command(cmd_node->args[0],
			ft_strlen(cmd_node->args[0])) == 1)
	{
		if ((*token_data)->syntax_error == 0)
			ft_perror_parsing(INVALID_COMMAND, cmd_node->args[0]);
		(*token_data)->syntax_error = 1;
	}
	cmd_node->args[i] = NULL;
}

t_ast	*parse_command(t_token_data **token_data, t_token **current)
{
	t_ast	*cmd_node;
	t_ast	*re_node;

	if (!(*current) || /* ((*current)->type != TOK_COMMAND
			&&  */!is_word((*current)->type))
	{
		re_node = parse_redirections(token_data, current);
		if (re_node != NULL)
			return (re_node);
		else if ((*token_data)->syntax_error == 0)
			ft_perror_parsing(SYNTAX_ERROR, (*current)->value);
		return ((*token_data)->syntax_error = 1, NULL);
	}
	cmd_node = create_ast_node(token_data, TOK_COMMAND);
	cmd_node->args = (char **)gc_malloc((*token_data)->gc, PARSING,
			sizeof(char *) * 10);
	parse_command_args(token_data, current, cmd_node);
	re_node = parse_redirections(token_data, current);
	if (re_node)
	{
		re_node->right = cmd_node;
		return (re_node);
	}
	return (cmd_node);
}
