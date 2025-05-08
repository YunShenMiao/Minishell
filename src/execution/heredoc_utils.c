/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:22:54 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/08 21:09:53 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_itoa_simple(int n, char *buf)
{
	int		len;
	char	tmp[12];
	int		i;
	int		j;

	if (n == 0)
	{
		buf[0] = '0';
		buf[1] = '\0';
		return ;
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

char	*generate_heredoc_filename(t_gc *gc, int id)
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
		return (get_next_line(STDIN_FILENO));
	}
	line = readline(prompt);
	if (!line)
		return (NULL);
	if (find_sign(line, '\n') >= 0)
	{
		buffered_lines = ft_split(line, '\n');
		free(line);
		current_index = 0;
		return (ft_strdup(buffered_lines[current_index++]));
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

void	cleanup_heredoc_tempfiles(int max_id)
{
	char	name[256];
	char	id_buf[12];
	int		i;
	char	*base;

	base = "/tmp/.heredoc_";
	i = 0;
	while (i < max_id)
	{
		ft_itoa_simple(i, id_buf);
		ft_strlcpy(name, base, ft_strlen(base) + 1);
		ft_strlcat(name, id_buf, ft_strlen(name) + ft_strlen(id_buf) + 1);
		unlink(name);
		i++;
	}
}
