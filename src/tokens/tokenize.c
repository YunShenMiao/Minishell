/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 09:41:57 by jwardeng          #+#    #+#             */
/*   Updated: 2025/03/26 15:14:55 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// still need to handle echo "" ""hello"" ("" is empty arg which is right but ""hello"" is also giving two emtpy args which needs to be fixed")
// export hello=$hello -> unset hello hello hello (unsets ignores rest)

void	quote_status(t_token_data **token_data, char input)
{
	if (input == '\'' && ((*token_data)->in_SQ) == 0
		&& ((*token_data)->in_DQ) == 0)
		(*token_data)->in_SQ = 1;
	else if (input == '\"' && ((*token_data)->in_SQ) == 0
		&& ((*token_data)->in_DQ) == 0)
		(*token_data)->in_DQ = 1;
	else if (input == '\'' && (*token_data)->in_SQ == 1
		&& ((*token_data)->in_DQ) == 0)
		(*token_data)->in_SQ = 0;
	else if (input == '\"' && ((*token_data)->in_SQ) == 0
		&& ((*token_data)->in_DQ) == 1)
		(*token_data)->in_DQ = 0;
}

int	end_quote(t_token_data **token_data)
{
	int		i;
	char	quote;

	i = (*token_data)->start;
	if ((*token_data)->in_SQ == 1)
		quote = '\'';
	else if ((*token_data)->in_DQ == 1)
		quote = '\"';
/* 	while(1)
	{ */
	while ((*token_data)->input[i] && (*token_data)->input[i] != quote)
		i++;
/* 	printf("i %d start %d\n", i, (*token_data)->start);
	if(i == (*token_data)->start && (*token_data)->input[i] != '\0' 
		&& (*token_data)->input[i + 1] != ' ')
		(*token_data)->start++;
		else
		break;
	} */
	if ((*token_data)->input[i] == '\0')
		return (printf("Error: unclosed quotes\n"), -1);
	return (i);
}

int	in_quote_token(t_token_data **token_data, int *i)
{
/* 	char quote;

	if ((*token_data)->in_SQ ==1)
	quote = '\'';
	else if ((*token_data)->in_DQ == 1)
	quote = '\"'; */
	(*i)++;
	(*token_data)->start = (*i);
	(*token_data)->end = end_quote(token_data);
	if ((*token_data)->end == -1)
		return (1);

	if (add_token(token_data) == 1)
		return (1);
	quote_status(token_data, (*token_data)->input[(*token_data)->end]);
	(*i) = (*token_data)->end + 1;
	if ((*token_data)->input[(*i) - 1] != '|')
		(*token_data)->first++;
	return (0);
}

int	in_token(t_token_data **token_data, int *i)
{
	(*token_data)->start = (*i);
	while ((*token_data)->input[(*i)] != ' ' && (*token_data)->input[(*i)] != '\"'
		&& (*token_data)->input[(*i)] != '\0' && (*token_data)->input[(*i)] != '\0')
		(*i)++;
	(*token_data)->end = (*i);
	if (add_token(token_data) == 1)
		return (1);
	if ((*token_data)->input[(*i) - 1] != '|')
		(*token_data)->first++;
	return (0);
}

int	tokenize(t_token_data **token_data)
{
	int	i;

	i = 0;
	while ((*token_data)->input[i] != '\0')
	{
		quote_status(token_data, (*token_data)->input[i]);
		if ((*token_data)->input[i] == ' ' && (*token_data)->in_SQ == 0
			&& (*token_data)->in_DQ == 0)
			i++;
		else if ((*token_data)->input[i] != ' ' && (*token_data)->in_SQ == 0
			&& (*token_data)->in_DQ == 0)
		{
			if (in_token(token_data, &i) == 1)
				return (1);
		}
		else if ((*token_data)->in_SQ == 1 || (*token_data)->in_DQ == 1)
		{
			if (in_quote_token(token_data, &i) == 1)
				return (1);
		}
	}
	return (0);
}
