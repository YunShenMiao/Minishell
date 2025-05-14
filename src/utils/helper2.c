/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 15:22:37 by jwardeng          #+#    #+#             */
/*   Updated: 2025/05/14 14:10:07 by jwardeng         ###   ########.fr       */
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
