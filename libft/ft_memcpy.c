/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 15:22:52 by xueyang           #+#    #+#             */
/*   Updated: 2024/10/14 14:53:12 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t len)
{
	unsigned char		*d;
	const unsigned char	*s;
	size_t				count;

	d = (unsigned char *)dst;
	s = (const unsigned char *)src;
	count = 0;
	if (d == NULL && s == NULL)
		return (NULL);
	while (count < len)
	{
		d[count] = s[count];
		count++;
	}
	return (dst);
}
