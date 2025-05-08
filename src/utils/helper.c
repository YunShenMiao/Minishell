/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 15:22:49 by jwardeng          #+#    #+#             */
/*   Updated: 2025/05/08 15:22:51 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void start_message(void)
{
    printf("\n______________________________________________________________________________\n");
    printf("   _____  .__       .__       .__           .__  .__       _____     ____  \n");
    printf("  /     \\ |__| ____ |__| _____|  |__   ____ |  | |  |     /      \\  |  o |\n");
    printf(" /  \\ /  \\|  |/    \\|  |/  ___/  |  \\_/ __ \\|  | |  |    |        |/ ___\\|\n");
    printf("/    Y    \\  |   |  \\  |\\___ \\|   Y  \\  ___/|  |_|  |__  |_________/ \n");
    printf("\\____|__  /__|___|  /__/____  >___|  /\\___  >____/____/  |_|_| |_|_|\n");
    printf("        \\/        \\/        \\/     \\/     \\/           [by xueyang & jwardeng]\n");
    printf("______________________________________________________________________________\n");
}

void	free_array(char **arr)
{
	int	i;

	i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

int	ft_put_to_fd(char *str, int fd)
{
	if (write(fd, str, ft_strlen(str)) < 0)
	{
		perror("write error");
		return (1);
	}
	return (0);
}

int ft_ministrcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return (1);
		s1++;
		s2++;
	}
	if (*s1 == '\0' && *s2 == '\0')
		return (0);
	return (1);
}
