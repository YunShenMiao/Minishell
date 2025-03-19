/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 17:04:24 by xueyang           #+#    #+#             */
/*   Updated: 2024/10/14 17:08:14 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	char	*address;
	char	*old_dest;
	int		len;

	len = 0;
	while (src[len] != '\0')
	{
		len++;
	}
	address = malloc(len + 1);
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
