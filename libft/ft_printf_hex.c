/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_hex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 22:33:31 by xueyang           #+#    #+#             */
/*   Updated: 2024/11/22 12:20:39 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_hex(unsigned long num, int uppercase)
{
	char	*base;
	char	buffer[16];
	size_t	len;
	size_t	final_len;

	if (num == 0)
		return (write(1, "0", 1));
	if (uppercase == 1)
		base = "0123456789ABCDEF";
	if (uppercase == 0)
		base = "0123456789abcdef";
	len = 0;
	while (num > 0)
	{
		buffer[len++] = base[num % 16];
		num = num / 16;
	}
	final_len = len;
	while (len--)
		if (write(1, &buffer[len], 1) == -1)
			return (-1);
	return (final_len);
}

int	ft_print_ptr(void *ptr)
{
	int	len;

	len = 0;
	if (!ptr)
		return (write(1, "0x0", 3));
	if (write(1, "0x", 2) == -1)
		return (-1);
	len = ft_print_hex((unsigned long)ptr, 0);
	if (len == -1)
		return (-1);
	return (2 + len);
}
