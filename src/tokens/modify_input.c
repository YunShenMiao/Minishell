/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modify_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 14:51:05 by jwardeng          #+#    #+#             */
/*   Updated: 2025/04/07 15:50:36 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	merge_echo_n2(char **edited, int *count, int *count2)
{
	(*edited)[*count] = 'e';
	(*edited)[*count + 1] = 'c';
	(*edited)[*count + 2] = 'h';
	(*edited)[*count + 3] = 'o';
	(*edited)[*count + 4] = '-';
	(*edited)[*count + 5] = 'n';
	(*count) += 6;
	(*count2) += 7;
}

// recognises each corresponding -n after echo command
// and merges to one without space
void	merge_echo_n(char *input, char **edited, int *count, int *count2)
{
	int	i;

	if (input[*count2] == 'e' && ft_strncmp(&input[*count2], "echo -n", 7) == 0
		&& (input[*count2 + 7] == ' ' || input[*count2 + 7] == 'n'
			|| input[*count2 + 7] == '\0'))
	{
		merge_echo_n2(edited, count, count2);
		while (input[*count2] != '\0')
		{
			if (ft_strncmp(&input[*count2], " -n", 3) == 0)
			{
				if (input[*count2 + 3] != ' ' && input[*count2 + 3] != 'n'
					&& input[*count2 + 3] != '\0')
					break ;
				(*count2) += 3;
			}
			i = 0;
			while (ft_strncmp(&input[*count2 + i], "n", 1) == 0)
				i++;
			if (input[*count2 + i] == ' ' || input[*count2 + i] == '\0')
				(*count2) += i;
			if (ft_strncmp(&input[*count2], " -n", 3) != 0)
				break ;
		}
	}
}

// adds extra spaces for redirections, heredoc and pipes which
// if inputted without space as delimiter
void	edit_spaces(char *input, char **edited, int *count, int *count2)
{
	if (input[*count2] == '>' || input[*count2] == '<' || input[*count2] == '|')
	{
		(*edited)[*count] = ' ';
		(*count)++;
		if (input[*count2] == input[*count2 + 1])
		{
			(*edited)[*count] = input[*count2];
			(*count2)++;
			(*count)++;
		}
	}
	(*edited)[*count] = input[*count2];
	if ((*edited)[*count] == '>' || (*edited)[*count] == '<'
		|| (*edited)[*count] == '|')
	{
		(*count)++;
		(*edited)[*count] = ' ';
	}
}

// trims extra quotes that bash ignores
void	trim_quotes(char *input, int *count2)
{
	int	count2cp;

	count2cp = 0;
	if (input[count2cp + *count2] == '\"')
	{
		while (input[count2cp + *count2] == '\"')
			count2cp++;
	}
	else if (input[*count2] == '\'')
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
int	modify_input(char *input, char **modified_input, t_gc *gc)
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
		merge_echo_n(input, modified_input, &count, &count2);
		trim_quotes(input, &count2);
		edit_spaces(input, modified_input, &count, &count2);
		count++;
		count2++;
	}
	(*modified_input)[count] = '\0';
	free(input);
	return (0);
}
