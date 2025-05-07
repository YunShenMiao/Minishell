/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 11:12:24 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/07 15:29:13 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_builtin(char **args)
{
	if (!args || !args[0])
		return (0);
	if (ft_ministrcmp(args[0], "echo") == 0 || ft_ministrcmp(args[0], "/bin/echo") == 0)
		return (1);
	if (ft_ministrcmp(args[0], "echo-n") == 0 || ft_ministrcmp(args[0], "/bin/echo-n") == 0)
		return (1);
	if (ft_ministrcmp(args[0], "cd") == 0 || ft_ministrcmp(args[0], "/bin/cd") == 0)
		return (1);
	if (ft_ministrcmp(args[0], "pwd") == 0)
		return (1);
	if (ft_ministrcmp(args[0], "export") == 0 || ft_ministrcmp(args[0], "/bin/export") == 0)
		return (1);
	if (ft_ministrcmp(args[0], "unset") == 0 || ft_ministrcmp(args[0], "/bin/unset") == 0)
		return (1);
	if (ft_ministrcmp(args[0], "env") == 0 || ft_ministrcmp(args[0], "/bin/env") == 0)
		return (1);
	if (ft_ministrcmp(args[0], "exit") == 0 || ft_ministrcmp(args[0], "/bin/exit") == 0)
		return (1);
	return (0);
}

//Sets up redirections for builtins (which run in the parent), save stdin or stdout for later for restoring
int setup_redirection(int input_fd, int output_fd, int *saved_stdin, int *saved_stdout)
{
	if (input_fd != STDIN_FILENO)
	{
		*saved_stdin = dup(STDIN_FILENO);
		if (*saved_stdin < 0)
			return (perror("dup"), -1);
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	if (output_fd != STDOUT_FILENO)
	{
		*saved_stdout = dup(STDOUT_FILENO);
		if (*saved_stdout < 0)
			return (perror("dup"), -1);
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
	return (0);
}

void restore_stdio(int saved_stdin, int saved_stdout)
{
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}

int	resolve_redirections(t_ast *node, int *in_fd, int *out_fd, t_token_data *td, t_ast **cmd_node)
{
	int	tmp_fd;

	if (!node)
		return (1);
	if (node->type == TOK_COMMAND)
	{
		*cmd_node = node;
		return (0);
	}
	if (node->type == TOK_REDIRECT_IN)
	{
		if (resolve_redirections(node->right, in_fd, out_fd, td, cmd_node) != 0)
			return (1);
		tmp_fd = open(node->left->file_name, O_RDONLY);
		if (tmp_fd < 0)
		{
			perror("open (in)");
			if (td->in_pipeline)
				exit(1);
			td->last_exit = 1;
			return (1);
		}
		if (*in_fd != STDIN_FILENO)
			close(*in_fd);
		*in_fd = tmp_fd;
		return (0);
	}
	else if (node->type == TOK_REDIRECT_OUT)
	{
		if (resolve_redirections(node->left, in_fd, out_fd, td, cmd_node) != 0)
			return (1);
		tmp_fd = open(node->right->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (tmp_fd < 0)
		{
			perror("open (out)");
			if (td->in_pipeline)
				exit(1);
			td->last_exit = 1;
			return (1);
		}
		if (*out_fd != STDOUT_FILENO)
			close(*out_fd);
		*out_fd = tmp_fd;
		return (0);
	}
	else if (node->type == TOK_APPEND)
	{
		if (resolve_redirections(node->left, in_fd, out_fd, td, cmd_node) != 0)
			return (1);

		tmp_fd = open(node->right->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (tmp_fd < 0)
		{
			perror("open (append)");
			if (td->in_pipeline)
				exit(1);
			td->last_exit = 1;
			return (1);
		}
		if (*out_fd != STDOUT_FILENO)
			close(*out_fd);
		*out_fd = tmp_fd;
		return (0);
	}
	return (1);
}

int	exec_ast(t_ast *node, int input_fd, int output_fd, t_token_data *td)
{
	pid_t	pid;
	int		pipefd[2];
	int		status;
	int		new_in;
	int		new_out;
	t_ast	*cmd_node;

	if (!node)
		return (1);
	if (node->type == TOK_PIPE)
	{
		if (pipe(pipefd) < 0)
			return (perror("pipe"), 1);
		pid = fork();
		if (pid == 0)
		{
			close(pipefd[0]);
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[1]);
			exec_ast(node->left, input_fd, STDOUT_FILENO, td);
			exit(td->last_exit);
		}
		close(pipefd[1]);
		td->in_pipeline = 1;
		exec_ast(node->right, pipefd[0], output_fd, td);
		td->in_pipeline = 0;
		close(pipefd[0]);
		waitpid(pid, &status, 0);
		return (td->last_exit);
	}
	else if (node->type == TOK_REDIRECT_IN || node->type == TOK_REDIRECT_OUT || node->type == TOK_APPEND)
	{
		new_in = input_fd;
		new_out = output_fd;
		cmd_node = NULL;
		if (resolve_redirections(node, &new_in, &new_out, td, &cmd_node) != 0)
			return (1);
		return (exec_ast(cmd_node, new_in, new_out, td));
	}	
	else if (node->type == TOK_COMMAND)
	{
		if (is_builtin(node->args) && !td->in_pipeline)
		{
			int saved_stdin = -1;
			int saved_stdout = -1;
		
			setup_redirection(input_fd, output_fd, &saved_stdin, &saved_stdout);
			td->last_exit = execute_builtins(node, td);
			restore_stdio(saved_stdin, saved_stdout);
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
