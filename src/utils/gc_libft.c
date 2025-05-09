/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_libft.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 15:18:30 by jwardeng          #+#    #+#             */
/*   Updated: 2025/05/09 16:23:42 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_env_substr(char const *s, unsigned int start, size_t len, t_gc *gc)
{
	char	*sub;
	size_t	count;

	if (start >= ft_strlen(s))
	{
		sub = gc_malloc(gc, ENV, 1);
		if (sub == NULL)
			return (NULL);
		sub[0] = '\0';
		return (sub);
	}
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	sub = gc_malloc(gc, ENV, len + 1);
	if (sub == NULL)
		return (NULL);
	count = 0;
	while (count < len)
	{
		sub[count] = s[start + count];
		count++;
	}
	sub[count] = '\0';
	return (sub);
}

char	*ft_env_strdup(const char *src, t_gc *gc, t_mem_location loc)
{
	char	*address;
	char	*old_dest;
	int		len;

	len = 0;
	while (src[len] != '\0')
	{
		len++;
	}
	address = gc_malloc(gc, loc, len + 1);
	old_dest = address;
	if (address == NULL)
		return (NULL);
	while (*src != '\0')
	{
		*address = *src;
		address++;
		src++;
	}
	*address = '\0';
	return (old_dest);
}

char	*ft_env_strjoin(char const *s1, char const *s2, t_gc *gc)
{
	int		i;
	int		j;
	char	*new;

	new = gc_malloc(gc, ENV, ft_strlen(s1) + ft_strlen(s2) + 1);
	if (new == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i] != '\0')
	{
		new[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		new[i + j] = s2[j];
		j++;
	}
	new[i + j] = '\0';
	return (new);
}

// modified strndup to use gc_malloc
// & have starting and end position for allocating
char	*ft_strndup(t_gc *gc, const char *src, int start, int end)
{
	char	*address;
	char	*old_dest;

	if (end - start < 0)
		return (NULL);
	address = (char *)gc_malloc(gc, TOKENS, end - start + 1);
	old_dest = address;
	if (address == NULL)
		return (NULL);
	while (src[start] != '\0' && start < end)
	{
		*address = src[start];
		address++;
		start++;
	}
	*address = '\0';
	return (old_dest);
}
