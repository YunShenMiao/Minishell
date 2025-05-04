/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 20:44:12 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/04 10:45:40 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * The AST is built so that the *last* pipe symbol (|) becomes the root. For each
 * PIPE node we always visit the left subtree first and then the right subtree –
 * that guarantees that the *first* command in the textual pipeline is executed
 * first, exactly as the user typed it.
 *
 *          ls | grep .c | wc -l               (textual order)
 *                   
 *                (PIPE) ─── right ──>  wc -l  (last command)
 *                   │
 *               left│
 *                   │
 *                (PIPE) ─── right ──> grep .c
 *                   │
 *               left│
 *                   │
 *                  ls                         (first command)
 *
 * The recursion always forks twice per PIPE node:
 *   1. The *left‑hand* child duplicates its stdout to the write end of the pipe
 *      and recurses on the left subtree.
 *   2. The *right‑hand* child duplicates its stdin to the read end of the pipe
 *      and recurses on the right subtree.
 *   3. The parent closes both ends of the pipe and waits for the two children.
 *
 */

static void	fatal_perror(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

//prev_read = -1 for the first one
int	exec_pipe(t_ast *node, int prev_read, t_token_data *td)
{
	int		fd[2];
	int		status;
	pid_t	pid;

	if (node->type == TOK_PIPE)
	{
		if (pipe(fd) == -1)
			return (perror("pipe"), 1);
		pid = fork();
		if (pid == -1)
			return (perror("fork"), 1);
		if (pid == 0)
		{
			if (prev_read != -1)
			{
				if (dup2(prev_read, STDIN_FILENO) == -1)
					fatal_perror("dup2");
				close(prev_read);
			}
			if (dup2(fd[1], STDOUT_FILENO) == -1)
				fatal_perror("dup2");
			close(fd[0]);
			close(fd[1]);
			exec_pipe(node->left, -1, td);
			_exit(1);
		}
		close(fd[1]);
		if (prev_read != -1)
			close(prev_read);
		status = exec_pipe(node->right, fd[0], td);
		if (waitpid(pid, &status, 0) == -1)
			perror("waitpid");
		else if (WIFEXITED(status))
			td->last_exit = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			td->last_exit = 128 + WTERMSIG(status);
		return (status);
	}
	else
	{
		//still need to fork the last command!
		pid = fork();
		if (pid == -1)
			return (perror("fork"), 1);
		if (pid == 0)
		{
			if (prev_read != -1)
			{
				if (dup2(prev_read, STDIN_FILENO) == -1)
					fatal_perror("dup2");
				close(prev_read);
			}
			exec_redir_pipe(node, &td, node->type, prev_read);
			_exit(1);
		}
		if (prev_read != -1)
			close(prev_read);
		if (waitpid(pid, &status, 0) == -1)
			perror("waitpid");
		else if (WIFEXITED(status))
			td->last_exit = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			td->last_exit = 128 + WTERMSIG(status);
		return (status);
	}
}

// int	exec_pipe(t_ast *node, int prev_read, t_token_data *td)
// {
// 	int		status;
// 	int		fd[2];
// 	pid_t	pid;

// 	if (!node)
// 		return (EXIT_FAILURE);
// 	if (node->type != TOK_PIPE)
// 		exec_redir_pipe(node, (&td), node->type, prev_read);
// 	status = 0;
// 	if (pipe(fd) == -1)
// 		return (perror("pipe"), 1);
// 	pid = fork();
// 	if (pid == -1)
// 		return (perror("fork"), 1);
// 	if (pid == 0)
// 	{
// 		close(fd[0]);
// 		if (prev_read != -1)
// 		{
// 			if (dup2(prev_read, STDIN_FILENO) == -1)
// 				fatal_perror("dup2");
// 			close(prev_read);
// 		}
// 		if (dup2(fd[1], STDOUT_FILENO) == -1)
// 			fatal_perror("dup2");
// 		close(fd[1]);
// 		exit(exec_pipe(node->left, -1, td)); //Execute left subtree and recursive
// 	}
// 	close(fd[1]);
// 	if (prev_read != -1)
// 		close(prev_read);
// 	status = exec_pipe(node->right, fd[0], td);
// 	if (waitpid(pid, &status, 0) == -1)
// 		perror("waitpid");
// 	else if (WIFEXITED(status))
// 		td->last_exit = WEXITSTATUS(status);
// 	else if (WIFSIGNALED(status))
// 		td->last_exit = 128 + WTERMSIG(status);
// 	return (status);
// }