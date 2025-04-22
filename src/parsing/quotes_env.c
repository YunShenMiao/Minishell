/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 13:51:18 by jwardeng          #+#    #+#             */
/*   Updated: 2025/04/22 16:46:14 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// checks for env key and if key has an assigned value
// if so copies the value instead of key to the new string
// if value invlaid just skips the key
void	expand_var(t_token_data **token_data, int *i, int *count, char *new)
{
	int		start;
	char	*value;
	char	*exitcode;
	if ((*token_data)->expand_str[*i] != '\0')
	(*i)++;
	start = (*i);
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
	}
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

// skipping dq, handling env expansion & copying rest
void	in_dq(t_token_data **token_data, char **new, int *i, int *count)
{
	if ((*token_data)->expand_str[*i] != '\0' && (*token_data)->expand_str[*i] == '\"')
		(*i)++;
	if ((*token_data)->expand_str[*i] == '$' && (*token_data)->expand_str[*i
			+ 1] != '\0' && (*token_data)->expand_str[*i + 1] != ' ' && (*token_data)->expand_str[*i + 1] != '\"')
		expand_var(token_data, i, count, *new);
	if ((*token_data)->expand_str[*i] != '\0' && (*token_data)->expand_str[*i] != '\"')
	{
		(*new)[*count] = (*token_data)->expand_str[*i];
		(*count)++;
		(*i)++;
	}
}

// copies everything to new until it finds the closing quote
// and skips initial & closing quote
void	in_sq(char **str, char **new, int *i, int *count)
{
	if ((*str)[*i] == '\'')
	(*i)++;
	while ((*str)[*i] != '\0' && (*str)[*i] != '\'')
	{
		(*new)[(*count)] = (*str)[(*i)];
		(*count)++;
		(*i)++;
	}
}

// handles env var & copies env var or just general values to the new string
void	in_nq(t_token_data **token_data, char **new, int *i, int *count)
{
	if ((*token_data)->expand_str[*i] == '$' && (*token_data)->expand_str[*i
			+ 1] != '\0' && (*token_data)->expand_str[*i + 1] != ' ')
	{
		expand_var(token_data, i, count, *new);
		return ;
	}
	if ((*token_data)->expand_str[*i] != '\"'
		&& (*token_data)->expand_str[*i] != '\'')
	{
		(*new)[*count] = (*token_data)->expand_str[*i];
		(*count)++;
	}
	if ((*token_data)->expand_str[*i] != '\0')
	(*i)++;
}

// allocates *new and trims the quotes inside command_args & file_name
// while tracking the quote_status and expanding $VAR inside NQ & DQ
// handles unclosed quotes error
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
	if ((*str)[i]!= '\0' && (quote_status(token_data, (*str)[i])) == 1)
		i++;
	while ((*str)[i] != '\0')
	{
		if (i >= (int)ft_strlen(*str)) // safety check (cast for good measure)
		break;
		quote_status(token_data, (*str)[i]);
		if ((*token_data)->in_DQ)
			in_dq(token_data, &new, &i, &count);
		else if ((*token_data)->in_SQ)
			in_sq(str, &new, &i, &count);
		else
			in_nq(token_data, &new, &i, &count);
	}
	if ((*token_data)->in_DQ == 1 || (*token_data)->in_SQ == 1)
		return ((ft_perror_parsing(token_data, UNCLOSED_QUOTES, "unclosed quotes")), NULL);
	return (new[count] = '\0', new);
}
