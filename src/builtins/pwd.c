/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:17:27 by xueyang           #+#    #+#             */
/*   Updated: 2025/03/23 16:17:12 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_pwd(t_token *current)
{
	char *cwd;

	if (current->next && current->next->type == TOK_WORD)
	{
		perror("pwd: too many arguments");
		return (1);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd failure");
		return (1);
	}
	if (ft_put_to_fd(cwd, 1) == 1)
		return (1);
	if (ft_put_to_fd("\n", 1) == 1)
		return (1);
	free(cwd);
	return (0);
}