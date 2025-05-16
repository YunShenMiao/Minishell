/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:21:25 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/16 18:34:22 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

// int	decrease_shlvl(t_token_data *td)
// {
// 	int		shlvl;
// 	char	*shlvl_str;
// 	t_env	*node;

// 	shlvl_str = search_name_val(td->env_list, "SHLVL");
// 	node = search_name_node(td->env_list, "SHLVL");
// 	if (shlvl_str == NULL || ft_ministrcmp(shlvl_str, "") == 0)
// 		shlvl = 1;
// 	else
// 	{
// 		shlvl = ft_atoi(shlvl_str);
// 		if (shlvl <= 1)
// 			shlvl = 1;
// 		else
// 			shlvl--;
// 	}
// 	if (node == NULL)
// 	{
// 		node = create_env("SHLVL", ft_itoa(shlvl), td->gc);
// 		if (!node)
// 			return (error_general("malloc: env not initiated\n"));
// 		ft_env_add_back(&td->env_list, node);
// 	}
// 	else
// 		node->val = ft_itoa(shlvl);
// 	return (0);
// }

int	ft_exit(char **args, t_token_data *td)
{
	int	exit_code;

	if (!args[1])
	{
		gc_free_all(td->gc, td->heredoc_id);
		free(td);
		_exit(EXIT_SUCCESS);
	}
	if (!is_numeric(args[1]))
	{
		ft_putendl_fd("exit: numeric argument required", 2);
		gc_free_all(td->gc, td->heredoc_id);
		free(td);
		_exit(255);
	}
	if (args[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		return (1);
	}
	exit_code = ft_atoi(args[1]) % 256;
	gc_free_all(td->gc, td->heredoc_id);
	free(td);
	_exit(exit_code);
}
