/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_norm_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 17:51:06 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/09 16:26:39 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*get_initial_path(const char *path, t_gc *gc)
{
	char	*temp;
	char	*result;

	if (path[0] == '/')
		return (ft_env_strdup("/", gc, BUILT_IN));
	temp = getcwd(NULL, 0);
	if (!temp)
		return (NULL);
	result = ft_env_strdup(temp, gc, BUILT_IN);
	free(temp);
	return (result);
}

static void	process_token(char *token, char **current_path, t_gc *gc)
{
	char	*temp;

	if (ft_strncmp(token, "..", 3) == 0)
	{
		temp = get_parent_dir(*current_path, gc);
		*current_path = temp;
	}
	else if (ft_strncmp(token, ".", 2) != 0 && token[0] != '\0')
	{
		if (ft_strlen(*current_path) > 1 && \
			(*current_path)[ft_strlen(*current_path) - 1] != '/')
		{
			temp = ft_env_strjoin(*current_path, "/", gc);
			*current_path = temp;
		}
		temp = ft_env_strjoin(*current_path, token, gc);
		*current_path = temp;
	}
}

char	*normalize_path(const char *path, t_gc *gc)
{
	char	*current_path;
	char	**tokens;
	int		i;

	current_path = get_initial_path(path, gc);
	if (!current_path)
		return (NULL);
	tokens = ft_split(path, '/');
	if (!tokens)
		return (NULL);
	i = 0;
	while (tokens[i])
	{
		process_token(tokens[i], &current_path, gc);
		free(tokens[i]);
		i++;
	}
	free(tokens);
	if (!current_path || ft_strlen(current_path) == 0)
		current_path = ft_env_strdup("/", gc, BUILT_IN);
	return (current_path);
}
