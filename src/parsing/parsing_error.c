/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 14:55:26 by jwardeng          #+#    #+#             */
/*   Updated: 2025/03/28 14:39:27 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// still need to handle:
// >>> when no quotes and spaces
// (eg. echo miao> >> output.txt || echo miao >>>output.txt)
// or: echo miao> > output.txt
// (zsh: parse error near '>>')

#include "../../include/minishell.h"

void	ft_perror_parsing(int error_id, char *error_info)
{
	if (error_id == INVALID_COMMAND)
		printf("🐢 minishell: '%s': command not found\n", error_info);
	if (error_id == SYNTAX_ERROR)
		printf("🐢 minishell: syntax error near unexpected token '%s'\n",
			error_info);
	if (error_id == UNCLOSED_QUOTES)
		printf("🐢 minishell: syntax error '%s'\n", error_info);
}

int	expecting_command(t_token *current, int *expecting_cmd)
{
	if (current->type == TOK_COMMAND || current->type == TOK_HEREDOC
		|| current->type == TOK_REDIRECT_IN || current->type == TOK_REDIRECT_OUT
		|| current->type == TOK_APPEND)
		(*expecting_cmd) = 0;
	else if ((current->type == TOK_PIPE && current->prev == NULL)
		|| (current->type == TOK_PIPE && current->prev->type == TOK_PIPE))
		return (ft_perror_parsing(SYNTAX_ERROR, current->value), 1);
	else
		return (ft_perror_parsing(INVALID_COMMAND, current->value), 1);
	return (0);
}

int	expecting_arg(t_token *current, int *expecting_cmd)
{
	if (current->type == TOK_PIPE)
	{
		(*expecting_cmd) = 1;
		if (current->next != NULL && current->next->type == TOK_PIPE)
			return (ft_perror_parsing(SYNTAX_ERROR, current->value), 1);
	}
	else if (current->type == TOK_REDIRECT_IN
		|| current->type == TOK_REDIRECT_OUT || current->type == TOK_APPEND)
	{
		if (current->next != NULL)
		{
			if (current->next->type != TOK_WORD_DQ
				&& current->next->type != TOK_WORD_SQ
				&& current->next->type != TOK_WORD_NQ)
				return (ft_perror_parsing(SYNTAX_ERROR, current->value), 1);
			current = current->next;
		}
	}
	else if (current->type == TOK_COMMAND)
		(*expecting_cmd) = 0;
	return (0);
}

int	syntax_error(t_token_data **token_data)
{
	t_token	*current;
	int		expecting_cmd;

	current = (*token_data)->token_list;
	expecting_cmd = 1;
	while (current != NULL)
	{
		if (expecting_cmd == 1)
		{
			if (expecting_command(current, &expecting_cmd) == 1)
				return (1);
		}
		else
		{
			if (expecting_arg(current, &expecting_cmd) == 1)
				return (1);
		}
		if ((current->next == NULL && current->type == TOK_REDIRECT_IN)
			|| (current->next == NULL && current->type == TOK_REDIRECT_OUT)
			|| (current->next == NULL && current->type == TOK_APPEND)
			|| (current->next == NULL && current->type == TOK_HEREDOC))
			return (ft_perror_parsing(SYNTAX_ERROR, "newline"), 1);
		current = current->next;
	}
	return (0);
}

/* int    parsing_error(t_token **token_list)
{
	t_token *temp;

	if (*token_list == NULL)
	return(-1);
	temp = (*token_list);
	while(temp != NULL && temp->next != NULL)
	{
		printf("miao1\n");
		if (temp->type == TOK_REDIRECT_IN || temp->type == TOK_REDIRECT_OUT
			|| temp->type == TOK_APPEND || temp->type == TOK_HEREDOC)
		{
			printf("miao2\n");
			if ((temp->next->type == TOK_REDIRECT_IN
					|| temp->next->type == TOK_REDIRECT_OUT
				|| temp->next->type == TOK_APPEND
					|| temp->next->type == TOK_HEREDOC))
			return(printf("parse error: REDIRECT/APPEND/HEREDOC\n"), -1);
		}
		printf("miao3\n");
		if(temp->next == NULL)
		break ;
		temp = temp->next;
	}
	printf("miao4\n");
return(1);
} */