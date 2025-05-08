/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:15:12 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/08 21:06:42 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	handle_redirect_in(t_ast *node, t_redi_ctx *ctx)
{
	int	tmp_fd;

	tmp_fd = open(node->left->file_name, O_RDONLY);
	if (tmp_fd < 0)
	{
		perror("open (in)");
		if (ctx->td->in_pipeline)
			exit(1);
		ctx->td->last_exit = 1;
		return (1);
	}
	if (*(ctx->in_fd) != STDIN_FILENO)
		close(*(ctx->in_fd));
	*(ctx->in_fd) = tmp_fd;
	return (0);
}

static int	handle_redirect_out(t_ast *node, t_redi_ctx *ctx)
{
	int	tmp_fd;

	tmp_fd = open(node->right->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmp_fd < 0)
	{
		perror("open (out)");
		if (ctx->td->in_pipeline)
			exit(1);
		ctx->td->last_exit = 1;
		return (1);
	}
	if (*(ctx->out_fd) != STDOUT_FILENO)
		close(*(ctx->out_fd));
	*(ctx->out_fd) = tmp_fd;
	return (0);
}

static int	handle_append(t_ast *node, t_redi_ctx *ctx)
{
	int	tmp_fd;

	tmp_fd = open(node->right->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (tmp_fd < 0)
	{
		perror("open (append)");
		if (ctx->td->in_pipeline)
			exit(1);
		ctx->td->last_exit = 1;
		return (1);
	}
	if (*(ctx->out_fd) != STDOUT_FILENO)
		close(*(ctx->out_fd));
	*(ctx->out_fd) = tmp_fd;
	return (0);
}

static int	resolve_redirect_type(t_ast *node, t_redi_ctx *ctx)
{
	if (node->type == TOK_REDIRECT_IN)
	{
		if (resolve_redirections(node->right, ctx))
			return (1);
		return (handle_redirect_in(node, ctx));
	}
	else if (node->type == TOK_REDIRECT_OUT)
	{
		if (resolve_redirections(node->left, ctx))
			return (1);
		return (handle_redirect_out(node, ctx));
	}
	else if (node->type == TOK_APPEND)
	{
		if (resolve_redirections(node->left, ctx))
			return (1);
		return (handle_append(node, ctx));
	}
	return (1);
}

int	resolve_redirections(t_ast *node, t_redi_ctx *ctx)
{
	if (!node)
		return (1);
	if (node->type == TOK_COMMAND)
	{
		*(ctx->cmd_node) = node;
		return (0);
	}
	return (resolve_redirect_type(node, ctx));
}
