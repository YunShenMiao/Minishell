/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:18:55 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/08 19:50:55 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	process_heredoc_node(t_ast *node, int *heredoc_id, t_token_data *td)
{
	char	*filename;

	filename = generate_heredoc_filename(td->gc, *heredoc_id);
	if (!filename)
	{
		perror("malloc heredoc");
		gc_free_all(td->gc, td->heredoc_id);
		exit(1);
	}
	if (write_to_file(node->left, filename, td) == -1)
	{
		perror("malloc heredoc");
		gc_free_all(td->gc, td->heredoc_id);
		exit(1);
	}
	node->left->file_name = filename;
	node->type = TOK_REDIRECT_IN;
	(*heredoc_id)++;
}

void	handle_all_heredocs(t_ast *node, int *heredoc_id, t_token_data *td)
{
	if (!node)
		return ;
	if (node->type == TOK_HEREDOC)
		process_heredoc_node(node, heredoc_id, td);
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

int	process_heredoc_line(int fd, char *line, t_ast *node, t_token_data *td)
{
	char	*expanded;

	expanded = expand_heredoc(line, node, td->env_list, td->last_exit);
	free(line);
	if (!expanded)
		return (-1);
	write(fd, expanded, strlen(expanded));
	write(fd, "\n", 1);
	free(expanded);
	return (0);
}

int	write_to_file(t_ast *node, char *file, t_token_data *td)
{
	int		fd;
	char	*line;

	fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
	{
		perror("heredoc: open");
		return (-1);
	}
	while (1)
	{
		line = read_heredoc_line();
		if (!line)
		{
			perror("heredoc: warning: EOF before delimiter\n");
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
	close(fd);
	return (0);
}
