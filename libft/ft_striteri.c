/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 14:44:07 by xueyang           #+#    #+#             */
/*   Updated: 2024/10/19 15:16:10 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	len_s;
	unsigned int	count;

	len_s = ft_strlen(s);
	count = 0;
	while (count < len_s)
	{
		f(count, s);
		count++;
		s++;
	}
}
