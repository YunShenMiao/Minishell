/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 10:03:33 by xueyang           #+#    #+#             */
/*   Updated: 2024/10/18 20:40:26 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	is_sep(char c, char const *charset)
{
	int	i;

	i = 0;
	while (charset[i] != '\0')
	{
		if (c == charset[i])
			return (1);
		i++;
	}
	return (0);
}

static	void	get_size(char const *s1, char const *set, int *start, int *end)
{
	while (s1[*start] && is_sep(s1[*start], set))
		(*start)++;
	while (*end > *start && is_sep(s1[*end - 1], set))
		(*end)--;
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*new;
	size_t	count_new;
	int		start;
	int		end;

	start = 0;
	end = ft_strlen(s1);
	get_size(s1, set, &start, &end);
	new = malloc((end) - (start) + 1);
	if (new == NULL)
		return (NULL);
	count_new = 0;
	while (start < end)
		new[count_new++] = s1[start++];
	new[count_new] = '\0';
	return (new);
}

/*
#include <stdio.h>
int main(void)
{
	printf("%s\n", ft_strtrim("ab cd  f    ", " "));
}
*/