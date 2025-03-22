/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 09:41:57 by jwardeng          #+#    #+#             */
/*   Updated: 2025/03/22 15:26:44 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	token_command(char *value, size_t len)
{
	if (ft_strncmp(value, "cd", len) == 0)
		return (1);
	else if (ft_strncmp(value, "echo", len) == 0)
		return (1);
	else if (ft_strncmp(value, "echo-n", len) == 0)
		return (1);
	else if (ft_strncmp(value, "pwd", len) == 0)
		return (1);
	else if (ft_strncmp(value, "env", len) == 0)
		return (1);
	else if (ft_strncmp(value, "export", len) == 0)
		return (1);
	if (ft_strncmp(value, "unset", len) == 0)
		return (1);
	else if (ft_strncmp(value, "exit", len) == 0)
		return (1);
	else
		return (-1);
}

t_tok_type	token_type(char *value)
{
	size_t	len;

	len = ft_strlen(value);
	if (ft_strncmp(value, ">", len) == 0)
		return (TOK_REDIRECT_OUT);
	else if (ft_strncmp(value, "<", len) == 0)
		return (TOK_REDIRECT_IN);
	else if (ft_strncmp(value, ">>", len) == 0)
		return (TOK_APPEND);
	else if (ft_strncmp(value, ">", len) == 0)
		return (TOK_HEREDOC);
	else if (ft_strncmp(value, "|", len) == 0)
		return (TOK_PIPE);
	else if (token_command(value, len) == 1)
		return (TOK_COMMAND);
	else
		return (TOK_WORD); 
	/* (printf("Syntax Error: Unexpected token"), -1); */
}

t_token	*create_token(char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	token->prev = NULL;
	token->next = NULL;
	if (token_type(value) == -1)
		return (NULL);
	token->type = token_type(value);
	return (token);
}

int	add_token(t_token **head, char *value)
{
	t_token	*new_token;
	t_token	*temp;

	new_token = create_token(value);
	if (!new_token)
		return (-1);
	if ((*head) == NULL)
	{
		(*head) = new_token;
		return (1);
	}
	temp = (*head);
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new_token;
	new_token->prev = temp;
	return (1);
}

void	print_list(t_token *head)
{
	t_token	*current;
	int		i;

	current = head;
	i = 1;
	printf("Doubly Linked List: ");
	while (current != NULL)
	{
		printf("%d: %s, type: %u\n", i, current->value, current->type);
		current = current->next;
		i++;
	}
}

int	tokenize(char **value, char *newinput, int words)
{
	t_token	*head;
	int		i;

	i = 0;
	head = NULL;
	while (i < words)
	{
		if (!add_token(&head, value[i]))
			return (free(newinput), -1);
		i++;
	}
	print_list(head);
	return (1);
}
