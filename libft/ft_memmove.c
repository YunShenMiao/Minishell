/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 15:35:59 by xueyang           #+#    #+#             */
/*   Updated: 2024/10/13 16:11:01 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*d;
	const unsigned char	*s;
	size_t				count;

	d = dst;
	s = src;
	if (d == s || len == 0)
		return (dst);
	if (d < s || d > s + len)
	{
		count = 0;
		while (count < len)
		{
			d[count] = s[count];
			count++;
		}
	}
	else
	{
		count = len;
		while (count-- > 0)
			d[count] = s[count];
	}
	return (dst);
}

/*
#include <string.h>

int	main()
{
	char	src[] = "Hello, World!";

	printf("%s\n", memmove(src, src+5, 5));
	printf("%s\n", ft_memmove(src, src+5, 5));
	return (0);
}
*/