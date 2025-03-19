/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 18:48:01 by xueyang           #+#    #+#             */
/*   Updated: 2024/10/14 09:55:31 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*ptr;
	size_t				count;

	ptr = s;
	count = 0;
	while (count < n)
	{
		if (ptr[count] == (unsigned char)c)
			return ((void *)(ptr + count));
		count++;
	}
	return (NULL);
}
