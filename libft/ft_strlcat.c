/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 16:16:48 by xueyang           #+#    #+#             */
/*   Updated: 2024/10/14 11:21:27 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dest_len;
	size_t	src_len;
	size_t	count;

	dest_len = 0;
	src_len = 0;
	count = 0;
	dest_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	if (dstsize <= dest_len)
		return (dstsize + src_len);
	while (count + dest_len < dstsize - 1 && src[count] != '\0')
	{
		dst[dest_len + count] = src[count];
		count++;
	}
	dst[dest_len + count] = '\0';
	return (dest_len + src_len);
}
