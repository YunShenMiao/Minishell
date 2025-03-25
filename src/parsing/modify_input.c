/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modify_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 14:51:05 by jwardeng          #+#    #+#             */
/*   Updated: 2025/03/25 10:36:08 by jwardeng         ###   ########.fr       */
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
		edit_spaces(input, modified_input, &count, &count2);
		count++;
		count2++;
	}
	(*modified_input)[count + 1] = '\0';
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
