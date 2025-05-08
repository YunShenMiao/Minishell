/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:30:12 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/08 15:55:08 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	error_general(char *msg)
{
	perror(msg);
	return (1);
}

int	error_free(char *msg, t_data *shell)
{
	perror(msg);
	// need to write more complete free under the struct
	if (shell)
		free(shell);
	return (1);
}

void	ft_perror_parsing2(t_token_data **token_data, int error_id,
		char *error_info)
{
	if (error_id == UNCLOSED_QUOTES)
	{
		write(2, "🐢 minishell: syntax error ", 29);
		write(2, error_info, ft_strlen(error_info));
		write(2, "\n", 2);
		(*token_data)->last_exit = 2;
	}
	else if (error_id == IS_DIR)
	{
		write(2, "🐢 minishell: ", 16);
		write(2, error_info, ft_strlen(error_info));
		write(2, ": is a directory\n", 17);
		(*token_data)->last_exit = 126;
	}
}

// customized parsing errors
void	ft_perror_parsing(t_token_data **token_data, int error_id,
		char *error_info)
{
	if (error_id == INVALID_COMMAND)
	{
		write(2, "🐢 minishell: ", 16);
		write(2, error_info, ft_strlen(error_info));
		write(2, ": command not found\n", 21);
		(*token_data)->last_exit = 127;
	}
	else if (error_id == SYNTAX_ERROR)
	{
		write(2, "🐢 minishell: syntax error near unexpected token '", 52);
		write(2, error_info, ft_strlen(error_info));
		write(2, "'\n", 2);
		(*token_data)->last_exit = 2;
	}
	else
		ft_perror_parsing2(token_data, error_id, error_info);
}
