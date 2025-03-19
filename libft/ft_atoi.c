/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 16:18:36 by xueyang           #+#    #+#             */
/*   Updated: 2024/10/14 16:54:29 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int	num;
	int	sign;

	sign = 1;
	num = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		if (num >= (2147483647 - (*str - '0')) / 10 && sign == 1)
			return (2147483647);
		if (sign == -1 && num >= (2147483648 - (*str - '0')) / 10)
			return (-2147483648);
		num = num * 10 + (*str - '0');
		str++;
	}
	return (sign * num);
}
