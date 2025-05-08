/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_token_data.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:14:39 by jwardeng          #+#    #+#             */
/*   Updated: 2025/05/08 15:50:03 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// initializes token_data which is passed during tokenization & parsing
// to acces modified input, set in-quote status, pass start
// & end position of token_values & pass garbage collector
// stuff for expand env var xx
int	init_token_data(char *input, t_token_data **token_data, t_gc *gc,
		char **envp)
{
	(*token_data)->input = input;
	(*token_data)->token_list = NULL;
	(*token_data)->ast = NULL;
	(*token_data)->in_SQ = 0;
	(*token_data)->in_DQ = 0;
	(*token_data)->start = 0;
	(*token_data)->end = 0;
	(*token_data)->gc = gc;
	(*token_data)->finish = 0;
	(*token_data)->envp = envp;
	(*token_data)->expand_str = NULL;
	(*token_data)->syntax_error = 0;
	(*token_data)->heredoc_id = 0;
	(*token_data)->in_pipeline = 0;
	(*token_data)->env_cmd = 0;
	(*token_data)->HD = 0;
	// (*token_data)->env_list = (t_env *)gc_malloc(gc, TOKENS, sizeof(t_env));
	// if (!(*token_data)->env_list)
	// 	return (1);
	return (0);
}
