/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 20:16:18 by xueyang           #+#    #+#             */
/*   Updated: 2024/10/20 20:41:45 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int		count;
	t_list	*temp;

	if (lst)
	{
		count = 1;
		temp = lst->next;
		while (temp != NULL)
		{
			temp = temp->next;
			count++;
		}
		return (count);
	}
	return (0);
}
