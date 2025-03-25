/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:10:15 by jwardeng          #+#    #+#             */
/*   Updated: 2025/03/25 10:39:02 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

t_tok_type	token_type(t_token_data **token_data, t_token *token)
{
	int len;

	len = ft_strlen(token->value);
	if (ft_strncmp(token->value, ">", len) == 0)
		return (TOK_REDIRECT_OUT);
	else if (ft_strncmp(token->value, "<", len) == 0)
		return (TOK_REDIRECT_IN);
	else if (ft_strncmp(token->value, ">>", len) == 0)
		return (TOK_APPEND);
	else if (ft_strncmp(token->value, "<<", len) == 0)
		return (TOK_HEREDOC);
	else if (ft_strncmp(token->value, "|", len) == 0)
		return (((*token_data)->first = 0), TOK_PIPE);
	else if (token_command(token->value, len) == 0 && (*token_data)->first == 0)
		return (TOK_COMMAND);
	else if ((*token_data)->in_DQ == 1)
		return (TOK_WORD_DQ);
	else if ((*token_data)->in_SQ == 1)
		return (TOK_WORD_SQ);
	else
		return (TOK_WORD_NQ); 
}

char *ft_strndup(const char *src, int start, int end)
{
	char	*address;
	char	*old_dest;

	address = malloc(end - start + 1);
	old_dest = address;
	if (address == NULL)
		return (NULL);
	while (src[start] != '\0' && start < end)
	{
		*address = src[start];
		address++;
		start++;
	}
	*address = '\0';
	return (old_dest);
}

t_token	*create_token(t_token_data **token_data)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strndup((*token_data)->input, (*token_data)->start, (*token_data)->end);
	token->prev = NULL;
	token->next = NULL;
	token->type = token_type(token_data, token);
	if (token->type == TOK_INVALID)
	return(NULL);
	return (token);
}

int	add_token(t_token_data **token_data)
{
	t_token	*new_token;
	t_token	*temp;

	new_token = create_token(token_data);
	if (!new_token)
		return (1);
	if ((*token_data)->token_list == NULL)
	{
		(*token_data)->token_list = new_token;
		return (0);
	}
	temp = (*token_data)->token_list;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new_token;
	new_token->prev = temp;
	return (0);
}
