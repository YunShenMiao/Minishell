/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:10:15 by jwardeng          #+#    #+#             */
/*   Updated: 2025/04/07 16:10:19 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_tok_type	token_type(t_token_data **token_data, t_token *token)
{
	int	len;

	len = ft_strlen(token->value);
	if (len == 0)
		return (TOK_EMPTY_WORD);
	if (ft_strncmp(token->value, ">", len) == 0)
		return (TOK_REDIRECT_OUT);
	else if (ft_strncmp(token->value, "<", len) == 0)
		return (TOK_REDIRECT_IN);
	else if (ft_strncmp(token->value, ">>", len) == 0)
		return (TOK_APPEND);
	else if (ft_strncmp(token->value, "<<", len) == 0)
		return (TOK_HEREDOC);
	else if (ft_strncmp(token->value, "|", len) == 0)
		return (TOK_PIPE);
	else if ((*token_data)->in_DQ == 1)
		return (TOK_WORD_DQ);
	else if ((*token_data)->in_SQ == 1)
		return (TOK_WORD_SQ);
	else
		return (TOK_WORD_NQ);
}

t_token	*create_token(t_token_data **token_data)
{
	t_token	*token;

	token = (t_token *)gc_malloc((*token_data)->gc, TOKENS, sizeof(t_token));
	if (!token)
		return (NULL);
	if ((*token_data)->finish == 1)
	{
		token->value = ft_strndup((*token_data)->gc, "newline", 0, 7);
		token->type = TOK_END;
	}
	else
	{
		token->value = ft_strndup((*token_data)->gc, (*token_data)->input,
				(*token_data)->start, (*token_data)->end);
		token->type = token_type(token_data, token);
	}
	if (!token->value)
		return (free(token), NULL);
	token->prev = NULL;
	token->next = NULL;
	if (token->type == TOK_INVALID)
		return (NULL);
	return (token);
}

// creates the simple token list out of the modified input string
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
