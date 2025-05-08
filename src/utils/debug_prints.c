/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_prints.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:55:18 by jwardeng          #+#    #+#             */
/*   Updated: 2025/05/08 17:05:06 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// just to check tokenization
void	print_list(t_token *head)
{
	t_token	*current;
	int		i;

	current = head;
	i = 1;
	printf("Doubly Linked List: \n");
	while (current != NULL)
	{
		printf("%d: [%s], type: %u\n", i, current->value, current->type);
		current = current->next;
		i++;
	}
}

void	print_ast2(t_ast *node, int *i)
{
	{
		printf("CMD: ");
		*i = 0;
		while (node->args[*i])
		{
			printf("%s ", node->args[*i]);
			(*i)++;
		}
	}
}

void	print_ast(t_ast *node, int depth, char *pos)
{
	int	i;

	if (!node)
		return ;
	i = 0;
	while (i < depth)
	{
		printf("  ");
		i++;
	}
	printf("%s ", pos);
	if (node->type == TOK_COMMAND)
		print_ast2(node, &i);
	else if (node->type == TOK_PIPE)
		printf("PIPE");
	else if (node->type == TOK_REDIRECT_IN || node->type == TOK_REDIRECT_OUT
		|| node->type == TOK_APPEND || node->type == TOK_HEREDOC)
		printf("REDIR: %d -> %s", node->type, node->file_name);
	else if (node->type == TOK_FILE)
		printf("FILE: %s", node->file_name);
	printf("\n");
	print_ast(node->left, depth + 1, "Left:");
	print_ast(node->right, depth + 1, "Right:");
}
