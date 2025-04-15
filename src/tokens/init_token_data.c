/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_token_data.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:14:39 by jwardeng          #+#    #+#             */
/*   Updated: 2025/04/15 21:31:37 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// initializes token_data which is passed during tokenization to access
// modified input, set in-quote status, pass start
// & end position of token_values & pass garbage collector
int	init_token_data(char *input, t_token_data **token_data,
	t_gc *gc, char **envp)
{
	(*token_data) = (t_token_data *)gc_malloc(gc, TOKENS, sizeof(t_token_data));
	if (!(*token_data))
		return (1);
	(*token_data)->input = input;
	(*token_data)->token_list = NULL;
	(*token_data)->ast = NULL;
	(*token_data)->in_SQ = 0;
	(*token_data)->in_DQ = 0;
	(*token_data)->start = 0;
	(*token_data)->end = 0;
	(*token_data)->gc = gc;
	(*token_data)->finish = 0;
	(*token_data)->syntax_error = 0;
	(*token_data)->envp = envp;
	(*token_data)->env_list = (t_env*)gc_malloc(gc, TOKENS, sizeof(t_env));
	if (!(*token_data)->env_list)
	return(1);
	(*token_data)->env_list = init_env(envp, gc);
	return (0);
}
