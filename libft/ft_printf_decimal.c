/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_decimal.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 22:46:16 by xueyang           #+#    #+#             */
/*   Updated: 2024/11/22 12:25:50 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_int(int num)
{
	char		*str;
	size_t		len;

	str = ft_itoa(num);
	if (!str)
		return (-1);
	len = ft_strlen(str);
	if (write(1, str, len) == -1)
	{
		free(str);
		return (-1);
	}
	free(str);
	return (len);
}

static char	*ft_utoa(unsigned int num)
{
	char			*str;
	size_t			len;
	unsigned int	temp;

	if (num == 0)
		return (ft_strdup("0"));
	temp = num;
	len = 0;
	while (temp > 0)
	{
		temp = temp / 10;
		len++;
	}
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	str[len] = '\0';
	while (num > 0)
	{
		str[--len] = (num % 10) + '0';
		num = num / 10;
	}
	return (str);
}

int	ft_print_unsign(unsigned int num)
{
	char		*str;
	size_t		len;

	str = ft_utoa(num);
	if (!str)
		return (-1);
	len = ft_strlen(str);
	if (write(1, str, len) == -1)
	{
		free(str);
		return (-1);
	}
	free(str);
	return (len);
}
