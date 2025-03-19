/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 10:22:57 by xueyang           #+#    #+#             */
/*   Updated: 2024/12/14 21:17:00 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <stddef.h>
// #include <limits.h>
#include <stdint.h>

#include "libft.h"

static void	free_split_array(char **array, size_t size)
{
	size_t	i;

	if (!array)
		return ;
	i = 0;
	while (i < size && array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

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

static char	*ft_strndup(const char *start, const char *end)
{
	char	*word;
	size_t	len;
	size_t	i;

	len = end - start;
	if (len > SIZE_MAX - 1)
		return (NULL);
	word = malloc(len + 1);
	if (!word)
		return (NULL);
	i = 0;
	while (start != end && i < len)
	{
		word[i] = *start;
		i++;
		start++;
	}
	word[i] = '\0';
	return (word);
}

static int	fill_array(const char *str, char c, char **array, size_t word_count)
{
	size_t		i;
	const char	*start;

	i = 0;
	start = NULL;
	while (*str || (start && i < word_count))
	{
		if (*str == c || *str == '\0')
		{
			if (start)
			{
				array[i] = ft_strndup(start, str);
				if (!array[i])
					return (i);
				i++;
				start = NULL;
			}
		}
		else if (!start)
			start = str;
		if (*str)
			str++;
	}
	array[i] = NULL;
	return (-1);
}

char	**ft_split(const char *str, char c)
{
	char	**array;
	size_t	word_count;
	int		result;

	if (!str)
		return (NULL);
	word_count = count_words(str, c);
	array = malloc((word_count + 1) * sizeof(char *));
	if (!array)
		return (NULL);
	result = fill_array(str, c, array, word_count);
	if (result >= 0)
	{
		free_split_array(array, result);
		return (NULL);
	}
	return (array);
}
