/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_basic.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 22:01:39 by xueyang           #+#    #+#             */
/*   Updated: 2024/11/22 12:05:26 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_char(int c)
{
	if (write(1, &c, 1) == -1)
		return (-1);
	return (1);
}

int	ft_print_str(char *str)
{
	if (!str)
		str = "(null)";
	if (write(1, str, ft_strlen(str)) == -1)
		return (-1);
	return (ft_strlen(str));
}

int	ft_print_percent(void)
{
	if (write(1, "%", 1) == -1)
		return (-1);
	return (1);
}
