/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:18:21 by xueyang           #+#    #+#             */
/*   Updated: 2025/03/23 21:19:01 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// cd haha >> output.txt 
// will print out the error message insetad of writing to output.txt, the .txt file will still be generated
// pwd is actually similar

char	*find_home(void)
{
	char	*home;

	home = get_env("HOME");
	if (!home)
	{
		perror("home not found");
		return (NULL);
	}
	return (home);
}

int	ft_cd(t_token *current)
{
	char *path;

	if (current->next)
	{
		path = current->next;
		if (path[0] == '~')
		{
			
		}
		if (chdir(path) == -1)
		{
			perror("cd: no such file or directory");
			return (1);
		}
	}
	else
		if (chdir(find_home()) == -1)
			return (1);
	return (0);
}