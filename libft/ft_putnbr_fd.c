/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 15:36:29 by xueyang           #+#    #+#             */
/*   Updated: 2024/10/19 16:10:56 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	void	check_special_case(int n, int fd)
{
	if (n == -2147483648)
	{
		write(fd, "-2147483648", 11);
	}
	if (n == 0)
	{
		write(fd, "0", 1.);
	}
}

void	ft_putnbr_fd(int n, int fd)
{
	char	str[12];
	int		count;
	int		is_negative;

	count = 0;
	is_negative = 0;
	check_special_case(n, fd);
	if (n == -2147483648 || n == 0)
		return ;
	if (n < 0)
	{
		is_negative = 1;
		n = -n;
	}
	while (n > 0)
	{
		str[count++] = (n % 10) + '0';
		n = n / 10;
	}
	if (is_negative == 1)
		str[count++] = '-';
	while (--count >= 0)
		write(fd, &str[count], 1);
}
