/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 21:10:21 by xueyang           #+#    #+#             */
/*   Updated: 2024/11/22 12:34:12 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	handle_specifier(char specifier, va_list args)
{
	int	result;

	if (specifier == 'c')
		result = ft_print_char(va_arg(args, int));
	else if (specifier == 's')
		result = (ft_print_str(va_arg(args, char *)));
	else if (specifier == 'd' || specifier == 'i')
		result = (ft_print_int(va_arg(args, int)));
	else if (specifier == 'u')
		result = (ft_print_unsign(va_arg(args, unsigned int)));
	else if (specifier == '%')
		result = (ft_print_percent());
	else if (specifier == 'p')
		result = (ft_print_ptr(va_arg(args, void *)));
	else if (specifier == 'x')
		result = (ft_print_hex(va_arg(args, unsigned int), 0));
	else if (specifier == 'X')
		result = (ft_print_hex(va_arg(args, unsigned int), 1));
	else
		result = 0;
	return (result);
}

static int	process_format(const char *format, va_list args)
{
	int		total_len;
	int		temp;

	total_len = 0;
	while (*format != '\0')
	{
		if (*format == '%')
		{
			format++;
			temp = handle_specifier(*format, args);
			if (temp == -1)
				return (-1);
			total_len += temp;
		}
		else
		{
			if (write(1, format, 1) == -1)
				return (-1);
			total_len += 1;
		}
		format++;
	}
	return (total_len);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		total_len;

	va_start(args, format);
	total_len = process_format(format, args);
	va_end(args);
	return (total_len);
}
