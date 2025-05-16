/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_libft.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 15:18:30 by jwardeng          #+#    #+#             */
/*   Updated: 2025/05/16 19:49:28 by jwardeng         ###   ########.fr       */
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

static	int	num_len(int n)
{
	int	len;

	len = 0;
	if (n == 0)
		len = 1;
	if (n < 0)
		len++;
	while (n != 0)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

char	*ft_env_itoa(t_gc *gc, int n)
{
	char	*str;
	int		len;

	len = num_len(n);
	str = gc_malloc(gc, ENV, len + 1);
	if (str == NULL)
		return (NULL);
	if (n < 0)
	{
		n = -n;
		str[0] = '-';
	}
	str[len] = '\0';
	while (n > 0)
	{
		str[--len] = (n % 10) + '0';
		n = n / 10;
	}
	return (str);
}
