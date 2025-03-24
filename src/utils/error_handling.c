/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:30:12 by xueyang           #+#    #+#             */
/*   Updated: 2025/03/24 14:34:15 by xueyang          ###   ########.fr       */
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
	//need to write more complete free under the struct
	if (shell)
		free(shell);
	return (1);
}