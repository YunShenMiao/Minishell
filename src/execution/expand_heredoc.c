/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 12:17:16 by jwardeng          #+#    #+#             */
/*   Updated: 2025/05/09 10:39:08 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_strnndup(const char *src, int start, int end)
{
	char	*address;
	char	*old_dest;

	if (end - start < 0)
		return (NULL);
	address = malloc(end - start + 1);
	old_dest = address;
	if (address == NULL)
		return (NULL);
	while (src[start] != '\0' && start < end)
	{
		*address = src[start];
		address++;
		start++;
	}
	*address = '\0';
	return (old_dest);
}

void	handle_hv_exitcode(char *expanded, int last_exit, int *count, int *i)
{
	char	*exitcode;

	exitcode = ft_itoa(last_exit);
	while (*exitcode)
	{
		expanded[*count] = *exitcode;
		(*count)++;
		exitcode++;
	}
	(*i)++;
}

char	*handle_here_var(char *line, t_env *env_list, int *i)
{
	int		start;
	char	*value;

	start = *i;
	while (line[*i] != '\0' && ((line[*i] < 123 && line[*i] > 96)
			|| (line[*i] < 91 && line[*i] > 47) || line[*i] == '_'))
		(*i)++;
	value = search_name_val(env_list, ft_strnndup(line, start, *i));
	return (value);
}

char	*expand_heredoc(char *line, t_env *env_list, int last_exit)
{
	int		count;
	int		i;
	char	*expanded;
	char	*value;

	count = 0;
	i = 0;
	expanded = malloc(ft_strlen(line) * 10);
	if (!expanded)
		return (NULL);
	while (line[i] != '\0')
	{
		if (line[i] == '$' && (line[i + 1] != '\0' && line[i + 1] != ' '
				&& line[i + 1] != '$'))
		{
			if (line[++i] == '?')
				handle_hv_exitcode(expanded, last_exit, &count, &i);
			value = handle_here_var(line, env_list, &i);
			while (value && *value)
				expanded[count++] = *value++;
		}
		else
			expanded[count++] = line[i++];
	}
	return (expanded[count] = '\0', expanded);
}
