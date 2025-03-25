/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modify_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 14:51:05 by jwardeng          #+#    #+#             */
/*   Updated: 2025/03/25 14:12:42 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	merge_echo_n(char *input, char **edited, int *count, int *count2)
{
	if (input[*count2] == 'e' && ft_strncmp(&input[*count2], "echo -n", 7) == 0)
	{
		(*edited)[*count] = 'e';
		(*edited)[*count + 1] = 'c';
		(*edited)[*count + 2] = 'h';
		(*edited)[*count + 3] = 'o';
		(*edited)[*count + 4] = '-';
		(*edited)[*count + 5] = 'n';
		(*count) += 6;
		(*count2) += 7;
		while (input[*count2] != '\0')
		{
			if (input[*count2] == ' ' && ft_strncmp(&input[*count2], " -n ",
					4) == 0)
				(*count2) += 3;
			else
				break ;
		}
	}
}

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

void	trim_quotes(char *input, int *count2)
{
	int count2cp;

	count2cp = 0;
	if (input[count2cp + *count2] == '\"')
	{
	while (input[count2cp + *count2] == '\"')
	count2cp++;
	}
	else if(input[*count2] == '\'')
	{
		while (input[count2cp] == '\'')
		count2cp++;
	}
	if (count2cp % 2 == 0 && count2cp > 1)
	(*count2) += (count2cp - 2);
	else if (count2cp > 2)
	(*count2) += (count2cp - 3);
}

int	edit_input(char *input, char **modified_input)
{
	int	count;
	int	count2;

	(*modified_input) = malloc(ft_strlen(input) * 2 + 1);
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
	return (0);
}

int	modify_input(char *input, char **modified_input)
{
	if ((edit_input(input, modified_input)) == 1)
		return (free(input), 1);
	free(input);
	/* 	if (parsing_error(head) == -1)
		return(-1); */
	return (0);
}
