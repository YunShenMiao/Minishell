/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:18:55 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/16 14:21:55 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// int	process_heredoc_node(t_ast *node, int *heredoc_id, t_token_data *td)
// {
// 	char	*filename;

// 	filename = generate_heredoc_filename(td->gc, *heredoc_id);
// 	if (!filename)
// 	{
// 		error_general("heredoc: failed to open temp file");
// 		gc_free_all(td->gc, td->heredoc_id);
// 		return (1);
// 	}
// 	if (write_to_file(node->left, filename, td) == -1)
// 	{
// 		perror("heredoc: failed to open temp file");
// 		gc_free_all(td->gc, td->heredoc_id);
// 		return (1);
// 	}
// 	node->left->file_name = filename;
// 	node->type = TOK_REDIRECT_IN;
// 	(*heredoc_id)++;
// 	return (0);
// }
int	process_heredoc_node(t_ast *node, int *heredoc_id, t_token_data *td)
{
	char	*filename;

	filename = generate_heredoc_filename(td->gc, *heredoc_id);
	if (!filename)
	{
		error_general("heredoc: failed to generate temp file");
		// gc_free_all(td->gc, td->heredoc_id);
		return (1);
	}
	if (write_to_file(node->left, filename, td) == -1)
	{
		td->heredoc_failed = 1;
		// gc_free_all(td->gc, td->heredoc_id);
		return (1);
	}
	node->left->file_name = filename;
	node->type = TOK_REDIRECT_IN;
	(*heredoc_id)++;
	return (0);
}

void	handle_all_heredocs(t_ast *node, int *heredoc_id, t_token_data *td)
{
	if (!node)
		return ;
	if (node->type == TOK_HEREDOC)
	{
		if (process_heredoc_node(node, heredoc_id, td) == 1)
		{
			td->heredoc_failed = 1;
			return;
		}
	}
	handle_all_heredocs(node->left, heredoc_id, td);
	handle_all_heredocs(node->right, heredoc_id, td);
}

char	*read_heredoc_line(void)
{
	char	*line;

	if (isatty(STDIN_FILENO))
		line = heredoc_readline("> ");
	else
	{
		line = get_next_line(STDIN_FILENO);
		if (line && line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
	}
	return (line);
}

int	write_heredoc_loop(int fd, t_ast *node, t_token_data *td)
{
	char	*line;

	while (1)
	{
		if (isatty(STDIN_FILENO))
		{
			write(1, "> ", 2);
			line = get_next_line(STDIN_FILENO);
			line = ft_env_substr(line, 0, ft_strlen(line) - 1, td->gc);
		}
		else
		{
			line = get_next_line(STDIN_FILENO);
			if (line && line[ft_strlen(line) - 1] == '\n')
				line[ft_strlen(line) - 1] = '\0';
		}
		if (!line)
		{
			// write(2, "heredoc: warning: EOF before delimiter\n", 39);
			break ;
		}
		if (ft_ministrcmp(line, node->file_name) == 0)
		{
			free(line);
			break ;
		}
		if (process_heredoc_line(fd, line, node, td) == -1)
			break ;
	}
	return (0);
}

int	write_to_file(t_ast *node, char *file, t_token_data *td)
{
	int		fd;
	pid_t	pid;
	int		status;
	int		is_interactive;

	is_interactive = isatty(STDIN_FILENO);
	fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
		return (perror("heredoc: open"), -1);
	if (!is_interactive)
	{
		write_heredoc_loop(fd, node, td);
		close(fd);
		return (0);
	}
	pid = fork();
	if (pid == -1)
		return (perror("heredoc: fork"), -1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);  // allow Ctrl-C to kill heredoc input
		write_heredoc_loop(fd, node, td);
		close(fd);
		exit(0);
	}
	else
	{
		close(fd);
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status))
			return (-1);
	}
	return (0);
}
