/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 10:34:10 by xueyang           #+#    #+#             */
/*   Updated: 2024/12/14 21:14:43 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	num_len(int n)
{
	int	len;

	len = 0;
	if (n == 0)
		len = 1;
	if (n < 0)
		len++;
	while (n != 0)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

static	char	*deal_special_case(int n)
{
	if (n == 0)
		return (ft_strdup("0"));
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	return (NULL);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		len;

	if (n == 0 || n == -2147483648)
		return (deal_special_case(n));
	len = num_len(n);
	str = malloc(len + 1);
	if (str == NULL)
		return (NULL);
	if (n < 0)
	{
		n = -n;
		str[0] = '-';
	}
	str[len] = '\0';
	while (n > 0)
	{
		str[--len] = (n % 10) + '0';
		n = n / 10;
	}
	return (str);
}
