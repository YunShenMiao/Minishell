/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:42:26 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/26 15:43:27 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
