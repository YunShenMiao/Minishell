/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 14:14:16 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/08 17:36:53 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_echo(char **args)
{
	int		no_newline;
	int		i;

	no_newline = 0;
	if (args[1] != NULL && ft_ministrcmp(args[1], "-n") == 0)
		no_newline = 1;
	else
		no_newline = 0;
	i = 0;
	if (no_newline == 1)
		i++;
	while (args[i + 1])
	{
		i++;
		if (ft_put_to_fd(args[i], 1) == 1)
			return (1);
		if (args[i + 1])
			if (ft_put_to_fd(" ", 1) == 1)
				return (1);
	}
	if (no_newline == 0)
		if (ft_put_to_fd("\n", 1) == 1)
			return (1);
	return (0);
}
