/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:17:27 by xueyang           #+#    #+#             */
/*   Updated: 2025/03/31 12:37:08 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_pwd(t_token *current)
{
	char	*cwd;
	
	if (current->next && (current->next->type == TOK_WORD_NQ || current->next->type == TOK_WORD_SQ || current->next->type == TOK_WORD_DQ))
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