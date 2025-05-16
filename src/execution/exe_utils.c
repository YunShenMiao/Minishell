/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 20:06:50 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/16 14:31:22 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	execute_builtins(t_ast *node, t_token_data *token_data)
{
	char	*value;

	value = node->args[0];
	if (ft_ministrcmp(value, "cd") == 0)
		return (ft_cd(node->args, (token_data)->env_list, (token_data)->gc));
	else if (ft_ministrcmp(value, "echo") == 0)
		return (ft_echo(node->args));
	else if (ft_ministrcmp(value, "pwd") == 0)
		return (ft_pwd(node->args));
	else if (ft_ministrcmp(value, "env") == 0)
		return (ft_env(node->args, (token_data)->env_list));
	else if (ft_ministrcmp(value, "export") == 0)
		return (ft_export((token_data)->env_list, node->args, token_data));
	else if (ft_ministrcmp(value, "unset") == 0)
		return (ft_unset((token_data)->env_list, node->args));
	else if (ft_ministrcmp(value, "exit") == 0)
		return (ft_exit(node->args, (token_data)));
	// else if (ft_ministrcmp(value, "./minishell") == 0)
	// 	return (shell_level(token_data));
	return (-1);
}

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
	// if (ft_ministrcmp(args[0], "./minishell") == 0)
	// 	return (1);
	return (0);
}

//Sets up redirections for builtins (which run in the parent)
int	setup_redi(int input_fd, int output_fd, int *s_stdin, int *s_stdout)
{
	if (input_fd != STDIN_FILENO)
	{
		*s_stdin = dup(STDIN_FILENO);
		if (*s_stdin < 0)
			return (perror("dup"), -1);
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	if (output_fd != STDOUT_FILENO)
	{
		*s_stdout = dup(STDOUT_FILENO);
		if (*s_stdout < 0)
			return (perror("dup"), -1);
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
	return (0);
}

void	restore_stdio(int saved_stdin, int saved_stdout)
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

int	exec_pipe(t_ast *node, int input_fd, int output_fd, t_token_data *td)
{
	pid_t	pid;
	int		pipefd[2];
	int		status;

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
