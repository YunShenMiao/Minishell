/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_waiting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 20:34:37 by xueyang           #+#    #+#             */
/*   Updated: 2025/03/19 21:17:46 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"

int main(void)
{
	char *input;
	char *prompt = GREEN "minishell" BLUE ">" RESET " ";

	start_message();
	while (1)
	{
		input = readline(prompt);
		if (!input)
		{
			printf("\n");
			break;
		}
		if (*input)
			add_history(input);
		free(input);
	}
	return (0);
}