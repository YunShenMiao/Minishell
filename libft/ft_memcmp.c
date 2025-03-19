/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 19:47:48 by xueyang           #+#    #+#             */
/*   Updated: 2024/10/14 09:52:53 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*ptr_1;
	const unsigned char	*ptr_2;
	size_t				count;

	ptr_1 = s1;
	ptr_2 = s2;
	count = 0;
	while (count < n)
	{
		if (ptr_1[count] != ptr_2[count])
			return (ptr_1[count] - ptr_2[count]);
		count++;
	}
	return (0);
}
