/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 15:22:37 by jwardeng          #+#    #+#             */
/*   Updated: 2025/05/14 17:13:08 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	empty_str(char *str)
{
	int	count;

	count = 0;
	while (str[count] != '\0' && (str[count] == ' ' || str[count] == '\t'
			|| str[count] == '\n' || str[count] == '\r' || str[count] == '\v'
			|| str[count] == '\f'))
		count++;
	if (str[count] != '\0')
		return (0);
	else
		return (count);
}

int	check_empty_ast(t_token_data *token_data)
{
	t_ast	*node;
	int		i;
	int		count;

	node = token_data->ast;
	i = 0;
	count = 0;
	if (node->left == NULL && node->right == NULL && node->type == TOK_COMMAND)
	{
		while (node->args[i] != NULL)
		{
			count = empty_str(node->args[i]);
			if (node->args[i][count] != '\0')
				return (0);
			i++;
		}
		if (node->args[i] == NULL)
		{
			return (2);
		}
	}
	return (0);
}

void	handle_empty_envp(char **envp, t_gc **gc)
{
	if (!envp[0])
	{
		char *pwd;
		char *temp;

		pwd = getcwd(NULL, 0);
		temp = ft_strjoin("PWD=", pwd);
		envp[0] = ft_env_strdup(temp, (*gc), ENV);
		free(pwd);
		free(temp);
		pwd = getcwd(NULL, 0);
		temp = ft_strjoin("OLDPWD=", pwd);
		envp[1] = ft_env_strdup(temp, (*gc), ENV);
		free(pwd);
		free(temp);
	}
}
