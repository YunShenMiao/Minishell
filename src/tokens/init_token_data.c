/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_token_data.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:14:39 by jwardeng          #+#    #+#             */
/*   Updated: 2025/03/27 15:36:44 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// initializes token_data which is passed during tokenization to access
// modified input, set in-quote status, pass start & end position of token_values
// have a signifier "first" for position beginning and after pipe & pass garbage collector
int	init_token_data(char *input, t_token_data **token_data, t_gc *gc)
{
	(*token_data) = (t_token_data *)gc_malloc(gc, TOKENS, sizeof(t_token_data));
	if (!(*token_data))
		return (1);
	(*token_data)->input = input;
	(*token_data)->token_list = NULL;
	(*token_data)->in_SQ = 0;
	(*token_data)->in_DQ = 0;
	(*token_data)->start = 0;
	(*token_data)->end = 0;
	(*token_data)->first = 0;
	(*token_data)->gc = gc;
	return (0);
}
