/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_prints.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:55:18 by jwardeng          #+#    #+#             */
/*   Updated: 2025/04/16 12:55:33 by jwardeng         ###   ########.fr       */
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

void print_ast(t_ast *node, int depth, char *pos)
{
    int i; 

	if (!node)
        return;
	i = 0;
    while (i < depth)
	{
        printf("  ");
		i++;
	}
	printf("%s ", pos);
    if (node->type == TOK_COMMAND)
    {
        printf("CMD: ");
		i = 0;
        while(node->args[i])
		{
            printf("%s ", node->args[i]);
			i++;
		}
    }
    else if (node->type == TOK_PIPE)
        printf("PIPE");
    else if (node->type == TOK_REDIRECT_IN || node->type == TOK_REDIRECT_OUT
			|| node->type == TOK_APPEND || node->type == TOK_HEREDOC)
        printf("REDIR: %d (fd=%d) -> %s", node->type, node->fd, node->file_name);
    printf("\n");

    print_ast(node->left, depth + 1, "Left:");
    print_ast(node->right, depth + 1, "Right:");
}

void	test(t_token_data **token_data, char **envp)
{
	t_ast *tree;

	tree = (*token_data)->ast;
	pid_t pid = fork();
	if (pid == 0) { 
		execve(tree->cmd_path, tree->args, envp);
    perror("execve failed");
    exit(1);
	} 
	else {  
    waitpid(pid, NULL, 0); 
	}
/* 	if (tree->type == TOK_COMMAND)
	execve(tree->cmd_path, tree->args, envp); */
}