/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 21:18:07 by xueyang           #+#    #+#             */
/*   Updated: 2024/12/10 14:48:48 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include "libft.h"
# include <unistd.h>
# include <stdlib.h>

int	ft_printf(const char *format, ...);
int	ft_print_char(int c);
int	ft_print_str(char *str);
int	ft_print_percent(void);
int	ft_print_int(int num);
int	ft_print_unsign(unsigned int num);
int	ft_print_hex(unsigned long num, int uppercase);
int	ft_print_ptr(void *ptr);

#endif