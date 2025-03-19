/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 21:07:43 by xueyang           #+#    #+#             */
/*   Updated: 2024/10/14 16:54:40 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	char	*hay;
	char	*ndl;
	size_t	count;
	size_t	i;

	if (*needle == '\0')
		return ((char *)haystack);
	count = 0;
	while (*haystack != '\0' && count < len)
	{
		hay = (char *)haystack;
		ndl = (char *)needle;
		i = 0;
		while (hay[i] == ndl[i] && ndl[i] != '\0' && i + count < len)
			i++;
		if (ndl[i] == '\0')
			return ((char *)haystack);
		haystack++;
		count++;
	}
	return (NULL);
}
