/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:17:27 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/08 17:08:23 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_pwd(char **args)
{
	char	*cwd;

	if (ft_ministrcmp(args[0], "pwd") != 0)
		return (-1);
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		return (error_general("getcwd failed"));
	}
	if (ft_put_to_fd(cwd, 1) == 1 || ft_put_to_fd("\n", 1) == 1)
	{
		free(cwd);
		return (1);
	}
	free(cwd);
	return (0);
}
