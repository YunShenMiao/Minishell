/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modify_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 14:51:05 by jwardeng          #+#    #+#             */
/*   Updated: 2025/03/23 19:07:41 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static size_t	count_words(const char *str, char c)
{
	size_t	count;
	size_t	i;
	int		in_word;

	count = 0;
	i = 0;
	in_word = 0;
	while (str[i])
	{
		if (str[i] == c)
			in_word = 0;
		else if (!in_word)
		{
			count++;
			in_word = 1;
		}
		i++;
	}
	return (count);
}

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
		while(input[*count2] != '\0')
		{
		if (input[*count2] == ' ' && ft_strncmp(&input[*count2], " -n ", 4) == 0)
		(*count2) += 3;
		else
		break;
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

char	*edit_input(char *input)
{
	char	*edited;
	int		count;
	int		count2;

	edited = malloc(ft_strlen(input) * 2 + 1);
	if (!edited)
		return (NULL);
	count = 0;
	count2 = 0;
	while (input[count2] != '\0')
	{
		merge_echo_n(input, &edited, &count, &count2);
		edit_spaces(input, &edited, &count, &count2);
		count++;
		count2++;
	}
	edited[count + 1] = '\0';
	return (edited);
}

int	modify_input(char *input, t_token **head)
{
	char	*newinput;
	char	**value;
	int		i;
	int		words;

	if ((newinput = edit_input(input)) == NULL)
		return (free(input), 1);
	free(input);
	value = ft_split(newinput, ' ');
	i = 0;
	words = count_words(newinput, ' ');
	if (tokenize(value, newinput, words, head) == 1)
		return (1);
/* 	if (parsing_error(head) == -1)
	return(-1); */
	free(newinput);
	return (0);
}
