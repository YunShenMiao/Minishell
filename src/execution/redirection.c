/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 20:44:04 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/07 15:20:57 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// redirect output overwrite>
int	red_out_overwrite(char *filename)
{
	int fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return(perror("open error"), 1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (1);
	close(fd);
	return (0);
}

// redirect output append >>
int	red_out_append(char *filename)
{
	int fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return(perror("open error"), 1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (1);
	close(fd);
	return (0);
}

// redirect input <
int	red_in(char *filename)
{
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (perror("open error"), 1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (1);
	close(fd);
	return (0);
}

// int heredoc(char *delimiter)
// {
// 	int fd[2];
// 	char *line;

// 	if (pipe(fd) == -1)
// 	{
// 		perror("pipe error");
// 		return (-1);
//     }
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line || (ft_strlen(line) == ft_strlen(delimiter) && ft_strncmp(line, delimiter, ft_strlen(line)) == 0))
// 		{
// 			free(line);
// 			break;
// 		}
// 		write(fd[1], line, ft_strlen(line));
// 		write(fd[1], "\n", 1);
// 		free(line);
// 	}
// 	close(fd[1]);
// 	return (fd[0]);
// }
static void	ft_itoa_simple(int n, char *buf)
{
	int		len;
	char	tmp[12];
	int		i;
	int		j;

	if (n == 0)
	{
		buf[0] = '0';
		buf[1] = '\0';
		return;
	}
	i = 0;
	while (n > 0)
	{
		tmp[i++] = (n % 10) + '0';
		n /= 10;
	}
	len = i;
	j = 0;
	while (j < len)
	{
		buf[j] = tmp[len - j - 1];
		j++;
	}
	buf[len] = '\0';
}

static char	*generate_heredoc_filename(t_gc *gc, int id)
{
	char	id_buf[12];
	char	*filename;
	char	*base;
	int		len;

	base = "/tmp/.heredoc_";
	ft_itoa_simple(id, id_buf);
	len = ft_strlen(base) + ft_strlen(id_buf) + 1;
	filename = gc_malloc(gc, EXECUTION, len);
	if (!filename)
		return (NULL);
	ft_strlcpy(filename, base, ft_strlen(base) + 1);
	ft_strlcat(filename, id_buf, ft_strlen(filename) + ft_strlen(id_buf) + 1);
	return (filename);
}

void	handle_all_heredocs(t_ast *node, int *heredoc_id, t_token_data *td)
{
	char	*filename;

	if (!node)
		return;
	if (node->type == TOK_HEREDOC)
	{
		filename = generate_heredoc_filename(td->gc, *heredoc_id);
		if (!filename)
		{
			perror("malloc heredoc");
			gc_free_all(td->gc, td->heredoc_id);
			exit(1);
		}
		if (write_heredoc_to_file(node->left, filename, td->env_list, td->last_exit) == -1)
		{
			perror("malloc heredoc");
			gc_free_all(td->gc, td->heredoc_id);
			exit(1);
		}
		// free(node->left->file_name);
		node->left->file_name = filename;
		node->type = TOK_REDIRECT_IN;
		(*heredoc_id)++;
	}
	handle_all_heredocs(node->left, heredoc_id, td);
	handle_all_heredocs(node->right, heredoc_id, td);
}

char	*heredoc_readline(const char *prompt)
{
	static char	**buffered_lines;
	static int	current_index;
	char		*line;

	if (buffered_lines && buffered_lines[current_index])
	{
		return (ft_strdup(buffered_lines[current_index++]));
	}
	if (!isatty(STDIN_FILENO))
	{
		return get_next_line(STDIN_FILENO);
	}
	line = readline(prompt);
	if (!line)
		return NULL;
	if (find_sign(line, '\n') >= 0)
	{
		buffered_lines = ft_split(line, '\n');
		free(line);
		current_index = 0;
		return (ft_strdup(buffered_lines[current_index++]));
	}
	return (line);
}

int	write_heredoc_to_file(t_ast *node, const char *filename, t_env *env_list, int last_exit)
{
	int		fd;
	char	*line;
	char	*expanded;
	
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
	{
		perror("heredoc: open");
		return (-1);
	}
	while (1)
	{
		if (isatty(STDIN_FILENO))
			line = heredoc_readline("> ");
		else
		{
			line = get_next_line(STDIN_FILENO);
			if (line && line[ft_strlen(line) - 1] == '\n')
				line[ft_strlen(line) - 1] = '\0';
		}
		if (!line)
		{
			perror("heredoc: warning: EOF before delimiter\n");
			break;
		}
		if (ft_ministrcmp(line, node->file_name) == 0)
		{
			free(line);
			break;
		}
		else
		{
		expanded = expand_heredoc(line, node, env_list, last_exit);
		free(line);
		write(fd, expanded, strlen(expanded));
		write(fd, "\n", 1);
		free(expanded);
		}
	}
	close(fd);
	return (0);
}

void	cleanup_heredoc_tempfiles(int max_id)
{
	char	filename[256];
	char	id_buf[12];
	int		i;
	char	*base;

	base = "/tmp/.heredoc_";
	i = 0;
	while (i < max_id)
	{
		ft_itoa_simple(i, id_buf);
		ft_strlcpy(filename, base, ft_strlen(base) + 1);
		ft_strlcat(filename, id_buf, ft_strlen(filename) + ft_strlen(id_buf) + 1);
		unlink(filename);
		i++;
	}
}
