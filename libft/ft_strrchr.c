/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 18:28:57 by xueyang           #+#    #+#             */
/*   Updated: 2024/10/13 18:47:20 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*updated_ptr;

	updated_ptr = NULL;
	while (*s != '\0')
	{
		if (*s == (char)c)
			updated_ptr = (char *)s;
		s++;
	}
	if ((char)c == '\0')
		updated_ptr = (char *)s;
	return (updated_ptr);
}
