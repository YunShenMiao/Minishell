/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 11:12:24 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/04 11:12:40 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int exec_ast(t_ast *node, int input_fd, int output_fd, char **envp)
{
	if (!node)
		return (1);

	if (node->type == TOK_PIPE)
	{
		int pipefd[2];
		if (pipe(pipefd) < 0)
			return (perror("pipe"), 1);

		if (fork() == 0)
		{
			close(pipefd[0]);
			exec_ast(node->left, input_fd, pipefd[1], envp);
			exit(1);
		}
		if (fork() == 0)
		{
			close(pipefd[1]);
			exec_ast(node->right, pipefd[0], output_fd, envp);
			exit(1);
		}
		close(pipefd[0]);
		close(pipefd[1]);
		wait(NULL);
		wait(NULL);
		return (0);
	}
	else if (node->type == TOK_REDIRECT_IN || node->type == TOK_REDIRECT_OUT || node->type == TOK_APPEND)
	{
		int fd;
		if (node->type == TOK_REDIRECT_IN)
			fd = open(node->left->file_name, O_RDONLY);
		else if (node->type == TOK_REDIRECT_OUT)
			fd = open(node->right->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			fd = open(node->right->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);

		if (fd < 0)
			return (perror("open redirection"), 1);

		if (node->type == TOK_REDIRECT_IN)
			return exec_ast(node->right, fd, output_fd, envp);
		else
			return exec_ast(node->left, input_fd, fd, envp);
	}
	else if (node->type == TOK_COMMAND)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			if (input_fd != STDIN_FILENO)
			{
				dup2(input_fd, STDIN_FILENO);
				close(input_fd);
			}
			if (output_fd != STDOUT_FILENO)
			{
				dup2(output_fd, STDOUT_FILENO);
				close(output_fd);
			}
			execve(node->cmd_path, node->args, envp);
			perror("execve");
			exit(127);
		}
		else
		{
			waitpid(pid, NULL, 0);
			return 0;
		}
	}
	return 1;
}
