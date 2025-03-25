/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_token_data.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:14:39 by jwardeng          #+#    #+#             */
/*   Updated: 2025/03/25 15:58:30 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	init_token_data(char *input, t_token_data **token_data)
{
	(*token_data) = malloc(sizeof(t_token_data));
	if (!(*token_data))
		return (1);
	(*token_data)->input = input;
	(*token_data)->token_list = NULL;
	(*token_data)->in_SQ = 0;
	(*token_data)->in_DQ = 0;
	(*token_data)->start = 0;
	(*token_data)->end = 0;
	(*token_data)->first = 0;
	/* (*token_data)->gc = NULL; */
	return (0);
}
