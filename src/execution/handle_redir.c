/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:15:12 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/08 20:36:02 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	handle_redirect_in(t_ast *node, int *in_fd, t_token_data *td)
{
	int	tmp_fd;

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

static int	handle_redirect_out(t_ast *node, int *out_fd, t_token_data *td)
{
	int	tmp_fd;

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

static int	handle_append(t_ast *node, int *out_fd, t_token_data *td)
{
	int	tmp_fd;

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

int	resolve_redi(t_ast *node, int *in, int *out, t_token_data *td, t_ast **cmd)
{
	if (!node)
		return (1);
	if (node->type == TOK_COMMAND)
	{
		*cmd = node;
		return (0);
	}
	if (node->type == TOK_REDIRECT_IN)
	{
		if (resolve_redi(node->right, in, out, td, cmd))
			return (1);
		return (handle_redirect_in(node, in, td));
	}
	else if (node->type == TOK_REDIRECT_OUT)
	{
		if (resolve_redi(node->left, in, out, td, cmd))
			return (1);
		return (handle_redirect_out(node, out, td));
	}
	else if (node->type == TOK_APPEND)
	{
		if (resolve_redi(node->left, in, out, td, cmd))
			return (1);
		return (handle_append(node, out, td));
	}
	return (1);
}
