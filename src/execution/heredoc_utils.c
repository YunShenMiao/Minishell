/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:22:54 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/26 13:47:51 by xueyang          ###   ########.fr       */
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

int	process_heredoc_line(int fd, char *line, t_ast *node, t_token_data *td)
{
	char	*expanded;

	if (node->quote == 1)
		expanded = ft_strdup(line);
	else
		expanded = expand_heredoc(line, td->env_list, td->last_exit);
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

void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
}

void	heredoc_signal(void)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = handle_sigint_heredoc;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
}

int	write_heredoc_interactive(int fd, t_ast *node, t_token_data *td)
{
	pid_t	pid;
	int		status;

	// signal(SIGINT, SIG_IGN);
	heredoc_signal();
	pid = fork();
	if (pid == -1)
		return (perror("heredoc: fork"), -1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		write_heredoc_loop(fd, node, td);
		close(fd);
		exit(0);
	}
	close(fd);
	waitpid(pid, &status, 0);
	setup_interactive_signals();
	if (WIFSIGNALED(status))
		return (-1);
	return (0);
}
