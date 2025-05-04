/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 11:12:24 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/04 12:45:31 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_builtin(char **args)
{
	if (!args || !args[0])
		return (0);
	if (ft_ministrcmp(args[0], "echo") == 0)
		return (1);
	if (ft_ministrcmp(args[0], "cd") == 0)
		return (1);
	if (ft_ministrcmp(args[0], "pwd") == 0)
		return (1);
	if (ft_ministrcmp(args[0], "export") == 0)
		return (1);
	if (ft_ministrcmp(args[0], "unset") == 0)
		return (1);
	if (ft_ministrcmp(args[0], "env") == 0)
		return (1);
	if (ft_ministrcmp(args[0], "exit") == 0)
		return (1);
	return (0);
}

int	exec_ast(t_ast *node, int input_fd, int output_fd, t_token_data *td)
{
	pid_t	pid;
	int		pipefd[2];
	int		status;
	int		fd;

	if (!node)
		return (1);
	if (node->type == TOK_PIPE)
	{
		if (pipe(pipefd) < 0)
			return (perror("pipe"), 1);
		td->in_pipeline = 1;
		if (fork() == 0)
		{
			close(pipefd[0]);
			exec_ast(node->left, input_fd, pipefd[1], td);
			exit(1);
		}
		if (fork() == 0)
		{
			close(pipefd[1]);
			exec_ast(node->right, pipefd[0], output_fd, td);
			exit(1);
		}
		td->in_pipeline = 0;
		close(pipefd[0]);
		close(pipefd[1]);
		wait(NULL);
		wait(NULL);
		return (0);
	}
	else if (node->type == TOK_REDIRECT_IN || node->type == TOK_REDIRECT_OUT || node->type == TOK_APPEND)
	{
		if (node->type == TOK_REDIRECT_IN)
			fd = open(node->left->file_name, O_RDONLY);
		else if (node->type == TOK_REDIRECT_OUT)
			fd = open(node->right->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			fd = open(node->right->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
			return (perror("open redirection"), 1);
		if (node->type == TOK_REDIRECT_IN)
			return exec_ast(node->right, fd, output_fd, td);
		else
			return exec_ast(node->left, input_fd, fd, td);
	}
	else if (node->type == TOK_COMMAND)
	{
		if (is_builtin(node->args) && !td->in_pipeline)
		{
			td->last_exit = execute_builtins(node, td);
			return (td->last_exit);
		}
		pid = fork();
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
			if (is_builtin(node->args))
				exit(execute_builtins(node, td));
			else
				execve(node->cmd_path, node->args, td->envp);
			perror("execve");
			exit(127);
		}
		else
		{
			if (waitpid(pid, &status, 0) == -1)
				perror("waitpid");
			if (WIFEXITED(status))
				td->last_exit = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				td->last_exit = 128 + WTERMSIG(status);
			return (td->last_exit);
		}
	}
	return (1);
}
