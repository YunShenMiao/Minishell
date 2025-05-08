/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modify_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 14:51:05 by jwardeng          #+#    #+#             */
/*   Updated: 2025/05/08 15:45:51 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// adds extra spaces for redirections, heredoc and pipes which
// if inputted without space as delimiter
void	edit_spaces(t_token_data **token_data, char **edited, int *count, int *count2)
{
	char *input;

	input = (*token_data)->input;
	if ((input[*count2] == '>' || input[*count2] == '<' || input[*count2] == '|') && (*token_data)->in_DQ == 0 && (*token_data)->in_SQ == 0)
	{
		if (input[*count2 - 1] != '\"' && input[*count2 - 1] != '\'')
		{
			(*edited)[*count] = ' ';
			(*count)++;
		}
		if (input[*count2] == input[*count2 + 1])
		{
			(*edited)[*count] = input[*count2];
			(*count2)++;
			(*count)++;
		}
	}
	(*edited)[*count] = input[*count2];
	if (((*edited)[*count] == '>' || (*edited)[*count] == '<'
			|| (*edited)[*count] == '|') && (input[*count2 + 1] != '\"'
			&& input[*count2 + 1] != '\'') && (*token_data)->in_DQ == 0 && (*token_data)->in_SQ == 0)
	{
		(*count)++;
		(*edited)[*count] = ' ';
	}
}

// trims extra quotes that bash ignores
void	trim_quotes(char *input, int *count2, t_token_data **token_data)
{
	int	count2cp;

	count2cp = 0;
	if (input[count2cp + *count2] == '\"' && (*token_data)->in_SQ == 0)
	{
		while (input[count2cp + *count2] == '\"')
			count2cp++;
	}
	else if (input[*count2] == '\'' && (*token_data)->in_DQ == 0)
	{
		while (input[count2cp] == '\'')
			count2cp++;
	}
	if (count2cp % 2 == 0 && count2cp > 1)
		(*count2) += (count2cp - 2);
	else if (count2cp > 2)
		(*count2) += (count2cp - 3);
}

// edits input string to pre-handle some cases and simplify tokenization
// and parsing, cases being handled: "echo -n"/"echo -n -nn"
// | missing spaces: eg "miao>>output.txt"
// | quotes to ignore: eg """""" -> ""
int	modify_input(char *input, char **modified_input, t_gc *gc, t_token_data **token_data)
{
	int	count;
	int	count2;

	(*modified_input) = (char *)gc_malloc(gc, TOKENS, ft_strlen(input) * 3 + 1);
	if (!(*modified_input))
		return (1);
	count = 0;
	count2 = 0;
	while (input[count2] != '\0')
	{
		quote_status(token_data, input[count2]);
		trim_quotes(input, &count2, token_data);
		edit_spaces(token_data, modified_input, &count, &count2);
		count++;
		count2++;
	}
	(*modified_input)[count] = '\0';
	free(input);
	return (0);
}
