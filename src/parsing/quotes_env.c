/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 13:51:18 by jwardeng          #+#    #+#             */
/*   Updated: 2025/04/17 18:22:40 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	expand_var(t_token_data **token_data, int *i, int *count, char *new)
{
	int		start;
	char	*value;

	(*i)++;
	start = (*i);
	// if ((*str)[(*i) + 1] == '?')
	// 	// handle ? else
	while ((*token_data)->expand_str[*i] != '\0'
		&& (((*token_data)->expand_str[*i] < 123
				&& (*token_data)->expand_str[*i] > 96)
			|| ((*token_data)->expand_str[*i] < 91
				&& (*token_data)->expand_str[*i] > 47)))
		(*i)++;
	value = search_name_val((*token_data)->env_list,
			ft_strndup((*token_data)->gc, (*token_data)->expand_str, start,
				(*i)));
	if (value)
	{
		while (*value)
		{
			new[*count] = *value;
			(*count)++;
			value++;
		}
	}
}

void	in_dq(t_token_data **token_data, char **new, int *i, int *count)
{
	if ((*token_data)->expand_str[*i] == '\"')
	(*i)++;
	if ((*token_data)->expand_str[*i] == '$' && (*token_data)->expand_str[*i
			+ 1] != '\0' && (*token_data)->expand_str[*i + 1] != ' ')
		expand_var(token_data, i, count, *new);
	if ((*token_data)->expand_str[*i] != '\"')
	{
		(*new)[*count] = (*token_data)->expand_str[*i];
		(*count)++;
		(*i)++;
	}
}

void	in_sq(char **str, char **new, int *i, int *count)
{
	(*i)++;
	while ((*str)[*i] != '\'')
	{
		(*new)[(*count)] = (*str)[(*i)];
		(*count)++;
		(*i)++;
	}
}

void	in_nq(t_token_data **token_data, char **new, int *i, int *count)
{
	if ((*token_data)->expand_str[*i] == '$' && (*token_data)->expand_str[*i
			+ 1] != '\0' && (*token_data)->expand_str[*i + 1] != ' ')
			{
		expand_var(token_data, i, count, *new);
		return;
			}
	if ((*token_data)->expand_str[*i] != '\"'
		&& (*token_data)->expand_str[*i] != '\'')
	{
		(*new)[*count] = (*token_data)->expand_str[*i];
		(*count)++;
	}
	(*i)++;
}

char	*handle_quotes(t_token_data **token_data, char **str)
{
	int		i;
	char	*new;
	int		count;

	i = 0;
	count = 0;
	(*token_data)->expand_str = (*str);
	new = (char *)gc_malloc((*token_data)->gc, PARSING, ft_strlen(*str) * 4);
	if (!new)
		return (NULL);
	quote_status(token_data, (*str)[i]);
	if ((*token_data)->in_DQ == 1 || (*token_data)->in_SQ == 1)
		i++;
	while ((*str)[i] != '\0')
	{
		quote_status(token_data, (*str)[i]);
		if ((*token_data)->in_DQ)
			in_dq(token_data, &new, &i, &count);
		else if ((*token_data)->in_SQ)
			in_sq(str, &new, &i, &count);
		else
			in_nq(token_data, &new, &i, &count);
	}
	if ((*token_data)->in_DQ == 1 || (*token_data)->in_SQ == 1)
		return ((ft_perror_parsing(UNCLOSED_QUOTES, "unclosed quotes")), NULL);
	return (new[count] = '\0', new);
}
