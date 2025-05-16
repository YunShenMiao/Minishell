/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 11:12:24 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/16 17:11:55 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	exec_with_redirection(t_ast *node, int in, int out, t_token_data *td)
{
	int			new_in;
	int			new_out;
	t_ast		*cmd_node;
	t_redi_ctx	ctx;

	new_in = in;
	new_out = out;
	cmd_node = NULL;
	ctx.in_fd = &new_in;
	ctx.out_fd = &new_out;
	ctx.cmd_node = &cmd_node;
	ctx.td = td;
	if (resolve_redirections(node, &ctx) != 0)
		return (1);
	return (exec_ast(cmd_node, new_in, new_out, td));
}

int	exec_builtin_direct(t_ast *node, int in, int out, t_token_data *td)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = -1;
	saved_stdout = -1;
	setup_redi(in, out, &saved_stdin, &saved_stdout);
	td->last_exit = execute_builtins(node, td);
	restore_stdio(saved_stdin, saved_stdout);
	return (td->last_exit);
}

// void	copy_binary(const char *src, const char *dest)
// {
// 	char	buf[4096];
// 	int		in, out;
// 	ssize_t	read_bytes;

// 	in = open(src, O_RDONLY);
// 	if (in < 0)
// 		return ;
// 	out = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0755);
// 	if (out < 0)
// 	{
// 		close(in);
// 		return ;
// 	}
// 	while ((read_bytes = read(in, buf, sizeof(buf))) > 0)
// 		write(out, buf, read_bytes);
// 	close(in);
// 	close(out);
// }

void	exec_cmd_child(t_ast *node, int in, int out, t_token_data *td)
{
	char	**new_envp;

	if (in != STDIN_FILENO)
	{
		dup2(in, STDIN_FILENO);
		close(in);
	}
	if (out != STDOUT_FILENO)
	{
		dup2(out, STDOUT_FILENO);
		close(out);
	}
	if (is_builtin(node->args))
		exit(execute_builtins(node, td));
	if (ft_ministrcmp(node->args[0], "./minishell") == 0)
	{
		new_envp = convert_to_envp(td->env_list);
		execve("./minishell", node->args, new_envp);
	}
	else
		execve(node->cmd_path, node->args, td->envp);
	perror("execve");
	exit(127);
}

int	exec_command(t_ast *node, int in, int out, t_token_data *td)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
		exec_cmd_child(node, in, out, td);
	if (waitpid(pid, &status, 0) == -1)
		perror("waitpid");
	if (WIFEXITED(status))
		td->last_exit = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		td->last_exit = 128 + WTERMSIG(status);
	return (td->last_exit);
}

int	exec_ast(t_ast *node, int input_fd, int output_fd, t_token_data *td)
{
	if (!node)
		return (1);
	if (node->type == TOK_PIPE)
		return (exec_pipe(node, input_fd, output_fd, td));
	else if (node->type == TOK_REDIRECT_IN || node->type == TOK_REDIRECT_OUT \
		|| node->type == TOK_APPEND)
		return (exec_with_redirection(node, input_fd, output_fd, td));
	else if (node->type == TOK_COMMAND)
	{
		if (is_builtin(node->args) && !td->in_pipeline)
			return (exec_builtin_direct(node, input_fd, output_fd, td));
		return (exec_command(node, input_fd, output_fd, td));
	}
	return (1);
}
