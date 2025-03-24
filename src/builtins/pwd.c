/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:17:27 by xueyang           #+#    #+#             */
/*   Updated: 2025/03/24 15:17:48 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_pwd(t_data *shell)
{
	char	*cwd;
	t_token	*current;

	current = shell->cur_token;
	if (current->next && current->next->type == TOK_WORD)
		return (error_general("pwd: too many arguments"));
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (error_general("getcwd failed"));
	if (ft_put_to_fd(cwd, 1) == 1)
		return (1);
	if (ft_put_to_fd("\n", 1) == 1)
		return (1);
	free(cwd);
	return (0);
}