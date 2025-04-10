/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 09:41:57 by jwardeng          #+#    #+#             */
/*   Updated: 2025/04/08 16:16:46 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// export hello=$hello -> unset hello hello hello (unsets ignores rest)
// need to handle echo "miao"juhu
// echo miao"muh$VAR"juhu'y"e"s"$VAR'and"$VAR"so$VARon
// miaomuhjuhuy"e"s"$VARandso

// void	quote_status(t_token_data **token_data, char input)
// {
// 	if (input == '\'' && ((*token_data)->in_SQ) == 0
// 		&& ((*token_data)->in_DQ) == 0)
// 		(*token_data)->in_SQ = 1;
// 	else if (input == '\"' && ((*token_data)->in_SQ) == 0
// 		&& ((*token_data)->in_DQ) == 0)
// 		(*token_data)->in_DQ = 1;
// 	else if (input == '\'' && (*token_data)->in_SQ == 1
// 		&& ((*token_data)->in_DQ) == 0)
// 		(*token_data)->in_SQ = 0;
// 	else if (input == '\"' && ((*token_data)->in_SQ) == 0
// 		&& ((*token_data)->in_DQ) == 1)
// 		(*token_data)->in_DQ = 0;
// }

// int	end_quote(t_token_data **token_data, int i)
// {
// 	char	quote;

// 	if ((*token_data)->in_SQ == 1)
// 		quote = '\'';
// 	else if ((*token_data)->in_DQ == 1)
// 		quote = '\"';
// 	while ((*token_data)->input[i] && (*token_data)->input[i] != quote)
// 		i++;
// 	if ((*token_data)->input[i] == '\0')
// 		return (ft_perror_parsing(UNCLOSED_QUOTES, "unclosed quotes"), -1);
// 	return (i);
// }

// // fixed the quotes but very unellegantly, should change this at some point
// int	in_quote_token(t_token_data **token_data, int *i)
// {
// 	(*i)++;
// 	(*token_data)->start = (*i);
// 	(*token_data)->end = end_quote(token_data, (*token_data)->start);
// 	if ((*token_data)->end - (*token_data)->start == 0
// 		&& (*token_data)->input[(*token_data)->end + 1] != '\0'
// 		&& ((*token_data)->input[(*token_data)->end + 1] != ' '
// 			|| (*token_data)->input[(*token_data)->start - 2] != ' '))
// 	{
// 		quote_status(token_data, (*token_data)->input[(*token_data)->end]);
// 		(*i)++;
// 		return (0);
// 	}
// 	if ((*token_data)->end == -1)
// 		return (1);
// 	printf("s %d e %d\n", (*token_data)->start, (*token_data)->end);
// 	if (add_token(token_data) == 1)
// 		return (1);
// 	quote_status(token_data, (*token_data)->input[(*token_data)->end]);
// 	(*i) = (*token_data)->end + 1;
// 	return (0);
// }

// /* int test()
// {
// 	char quote;
	
// 	quote = (*token_data)->input[(*i)];
// 	while ((*token_data)->input[(*i)] != quote  && (*token_data)->input[(*i)] != '\0')
// 	(*i)++;
// } */

// int	in_token(t_token_data **token_data, int *i, int begin)
// {
// 	if (begin == 0)
// 		(*token_data)->start = *i;

// 	while ((*token_data)->input[*i] != '\0')
// 	{
// 		// If we hit a space, end of token
// 		if ((*token_data)->input[*i] == ' ')
// 			break;

// 		// If we hit a quote, handle quoted string
// 		if ((*token_data)->input[*i] == '\"' || (*token_data)->input[*i] == '\'')
// 		{
// 			char quote = (*token_data)->input[*i];
// 			quote_status(token_data, quote);
// 			(*i)++; // Skip opening quote

// 			// Loop until closing quote or end of string
// 			while ((*token_data)->input[*i] && (*token_data)->input[*i] != quote)
// 				(*i)++;

// 			if ((*token_data)->input[*i] == quote)
// 				(*i)++; // Skip closing quote

// 			continue;
// 		}

// 		// Regular character, keep going
// 		(*i)++;
// 	}

// 	(*token_data)->end = *i;

// 	if (add_token(token_data) == 1)
// 		return (1);
// 	return (0);
// }

// // int	in_token(t_token_data **token_data, int *i, int begin)
// // {
// // 	if (begin == 0)
// // 	(*token_data)->start = (*i);
// // 	while ((*token_data)->input[(*i)] != ' '
// // 		&& (*token_data)->input[(*i)] != '\"'
// // 		&& (*token_data)->input[(*i)] != '\''
// // 		&& (*token_data)->input[(*i)] != '\0')
// // 		(*i)++;
// // 	if (((*token_data)->input[(*i)] == '\"' || (*token_data)->input[(*i)] == '\'')
// // 		&& (*token_data)->input[(*i) + 1] != ' ')
// // 	{
// // 		quote_status(token_data, (*token_data)->input[(*i)]);
// // 		(*i) = end_quote(token_data, (*i));
// // 		in_token(token_data, i, begin++);
// // 	}
// // /* 		while (1)
// // 		{
// // 		quote = (*token_data)->input[(*i)];
// // 		(*i)++;
// // 		while ((*token_data)->input[(*i)] != quote  && (*token_data)->input[(*i)] != '\0')
// // 		(*i)++;
// // 		if ((((*token_data)->input[(*i)] == '\"' || (*token_data)->input[(*i)] == '\'')
// // 			&& (*token_data)->input[(*i) + 1] == ' ') || (*token_data)->input[(*i)] == '\0')
// // 			break;
// // 		} */
// // 	(*token_data)->end = (*i);
// // 	if (add_token(token_data) == 1)
// // 		return (1);
// // 	return (0);
// // }

// decides if token is SQ DQ or NQ and skips spaces before calling add_token
// int	tokenize(t_token_data **token_data)
// {
// 	int	i;

// 	i = 0;
// 	printf("string: %s\n", (*token_data)->input);
// 	while ((*token_data)->input[i] != '\0')
// 	{
// 		quote_status(token_data, (*token_data)->input[i]);
// 		if ((*token_data)->input[i] == ' ' && (*token_data)->in_SQ == 0
// 			&& (*token_data)->in_DQ == 0)
// 			i++;
// 		else if ((*token_data)->input[i] != ' ' && (*token_data)->in_SQ == 0
// 			&& (*token_data)->in_DQ == 0)
// 		{
// 			if (in_token(token_data, &i, 0) == 1)
// 				return (1);
// 		}
// 		else if ((*token_data)->in_SQ == 1 || (*token_data)->in_DQ == 1)
// 		{
// 			if (in_quote_token(token_data, &i) == 1)
// 				return (1);
// 		}
// 	}
// 	(*token_data)->finish = 1;
// 	add_token(token_data);
// 	return (0);
// }

// int in_token(t_token_data **token_data, char *str, int *i)
// {
// 	(*token_data)->start = (*i);
// 	while (str[*i] != ' ' && str[*i] != '\0')
// 	(*i)++;
// 	(*token_data)->end = (*i);
// 	add_token(token_data);
// }

// echo miao"muh$VAR"juhu'yes$VAR'and"$VAR"so$VARon

