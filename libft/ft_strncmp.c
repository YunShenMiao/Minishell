/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 18:43:06 by xueyang           #+#    #+#             */
/*   Updated: 2024/10/14 11:28:31 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	count;

	count = 0;
	while (count < n && s1[count] != '\0' && s2[count] != '\0')
	{
		if ((unsigned char)s1[count] != (unsigned char)s2[count])
			return ((unsigned char)s1[count] - (unsigned char)s2[count]);
		count++;
	}
	if (count < n)
		return ((unsigned char)s1[count] - (unsigned char)s2[count]);
	return (0);
}
