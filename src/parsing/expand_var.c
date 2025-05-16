/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 18:04:03 by jwardeng          #+#    #+#             */
/*   Updated: 2025/05/16 13:39:33 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_var_cmd(t_token_data **token_data, int *count, char *value,
		char *new)
{
	if (*count == 0)
	{
		while (*value == ' ')
			value++;
	}
	while (*value)
	{
		if (*value == ' ' && (*token_data)->dq == 0)
		{
			((*token_data)->env_cmd = 1);
			new[*count] = *value;
			(*count)++;
			value++;
			while (*value && *value == ' ')
				value++;
		}
		if (*value)
		{
			new[*count] = *value;
			(*count)++;
			value++;
		}
	}
}

int	handle_var_exitcode(t_token_data **token_data, int *i, int *count,
		char *new)
{
	char	*exitcode;

	if ((*token_data)->expand_str[*i] == '?')
	{
		exitcode = ft_itoa((*token_data)->last_exit);
		while (*exitcode)
		{
			new[*count] = *exitcode;
			(*count)++;
			exitcode++;
		}
		(*i)++;
		return (0);
	}
	return (1);
}

// checks for env key and if key has an assigned value
// if so copies the value instead of key to the new string
// if value invlaid just skips the key
// NEED TO: find better way to handle '_' & var_cmd...
void	expand_var(t_token_data **token_data, int *i, int *count, char *new)
{
	int		start;
	char	*value;

	if ((*token_data)->expand_str[*i] == '~')
	{
		value = search_name_val((*token_data)->env_list, "HOME");
		(*i)++;
	}
	else
	{
		start = ++(*i);
		handle_var_exitcode(token_data, i, count, new);
		while ((*token_data)->expand_str[*i] != '\0'
			&& (ft_isalpha((*token_data)->expand_str[*i])
				|| ft_isdigit((*token_data)->expand_str[*i])
				|| (*token_data)->expand_str[*i] == '_'))
			(*i)++;
		value = search_name_val((*token_data)->env_list,
				ft_strndup((*token_data)->gc, (*token_data)->expand_str, start,
					(*i)));
	}
	if (value)
		handle_var_cmd(token_data, count, value, new);
}
