/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 13:51:18 by jwardeng          #+#    #+#             */
/*   Updated: 2025/05/16 17:11:43 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// skipping dq, handling env expansion & copying rest
void	in_dq(t_token_data **token_data, char **new, int *i, int *count)
{
	if ((*token_data)->expand_str[*i] != '\0'
		&& (*token_data)->expand_str[*i] == '\"')
		(*i)++;
	if (((*token_data)->expand_str[*i] == '$' && (*token_data)->expand_str[*i
				+ 1] != '\0' && (*token_data)->expand_str[*i + 1] != ' '
			&& (*token_data)->expand_str[*i + 1] != '\t'
			&& (*token_data)->expand_str[*i + 1] != '\"'
			&& (*token_data)->expand_str[*i + 1] != '/'
			&& (*token_data)->hd == 0))
		expand_var(token_data, i, count, *new);
	if ((*token_data)->expand_str[*i] != '\0'
		&& (*token_data)->expand_str[*i] != '\"')
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
	if (((*token_data)->expand_str[*i] == '$' && (*token_data)->expand_str[*i
				+ 1] != '\0' && (*token_data)->expand_str[*i + 1] != ' '
			&& (*token_data)->expand_str[*i + 1] != '/'
			&& (*token_data)->expand_str[*i + 1] != '\t'
			&& (*token_data)->hd == 0) || ((*token_data)->expand_str[*i] == '~'
			&& ((*token_data)->expand_str[*i + 1] == '\0'
				|| ((*token_data)->expand_str[*i + 1] == ' ')
				|| (*token_data)->expand_str[*i + 1] == '\t')))
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
	new = (char *)gc_malloc((*token_data)->gc, PARSING, 4096);
	if ((*str)[i] != '\0' && (quote_status(token_data, (*str)[i])) == 1)
		i++;
	while ((*str)[i] != '\0')
	{
		quote_status(token_data, (*str)[i]);
		if ((*token_data)->dq)
			in_dq(token_data, &new, &i, &count);
		else if ((*token_data)->sq)
			in_sq(str, &new, &i, &count);
		else
			in_nq(token_data, &new, &i, &count);
	}
	if ((*token_data)->dq == 1 || (*token_data)->sq == 1)
		return ((*token_data)->hd = 0, (ft_perror_parsing(token_data,
					UNCLOSED_QUOTES, "unclosed quotes")), NULL);
	(*token_data)->hd = 0;
	return (new[count] = '\0', new);
}
