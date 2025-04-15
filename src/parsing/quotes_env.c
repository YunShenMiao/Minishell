/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 13:51:18 by jwardeng          #+#    #+#             */
/*   Updated: 2025/04/15 15:04:31 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	expand_var(char **str, int *i, int *count, char **new)
{
	int start;

	start = (*i);
	//(check for var)
	// var valid -> add var to new increment i & count
	// var invalid -> just skip and increment i
	if ((*str)[(*i) + 1] == '?')
		// handle ?
		while ((*str)[*i] != '\0' && (*str)[*i] < 123 && (*str)[*i] > 96
			&& (*str)[*i] < 91 && (*str)[*i] > 47)
			(*i)++;
	(*new)[*count] = 'X';
	(*count)++;
}

void	in_dq(char **str, char **new, int *i, int *count)
{
	if ((*str)[*i] == '$' && (*str)[*i + 1] != '\0' && (*str)[*i + 1] != ' ')
		expand_var(str, i, count, new);
	if ((*str)[*i] != '\"')
	{
		(*new)[*count] = (*str)[*i];
		(*count)++;
	}
	(*i)++;
}

void	in_sq(char **str, char **new, int *i, int *count)
{
	if ((*str)[*i] != '\'')
		(*new)[(*count)++] = (*str)[(*i)];
	(*i)++;
}

void	in_nq(char **str, char **new, int *i, int *count)
{
	if ((*str)[*i] == '$' && (*str)[*i + 1] != '\0' && (*str)[*i + 1] != ' ')
		expand_var(str, i, count, new);
	if ((*str)[*i] != '\"' && (*str)[*i] != '\'')
	{
		(*new)[*count] = (*str)[*i];
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
			in_dq(str, &new, &i, &count);
		else if ((*token_data)->in_SQ)
			in_sq(str, &new, &i, &count);
		else
			in_nq(str, &new, &i, &count);
	}
	if ((*token_data)->in_DQ == 1 || (*token_data)->in_SQ == 1)
		return ((ft_perror_parsing(UNCLOSED_QUOTES, "unclosed quotes")), NULL);
	return (new[count] = '\0', new);
}
