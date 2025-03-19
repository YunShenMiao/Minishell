/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 18:06:54 by xueyang           #+#    #+#             */
/*   Updated: 2024/10/19 14:48:09 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	len_s;
	unsigned int	count;
	char			*new;

	len_s = ft_strlen(s);
	count = 0;
	if (s == NULL || f == NULL)
		return (NULL);
	new = malloc(len_s + 1);
	if (new == NULL)
		return (NULL);
	while (count < len_s)
	{
		new[count] = f(count, s[count]);
		count++;
	}
	new[count] = '\0';
	return (new);
}
