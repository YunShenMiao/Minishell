/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 20:06:50 by xueyang           #+#    #+#             */
/*   Updated: 2025/04/17 18:32:46 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	exec_redirs(t_ast *node)
{
	int hd;

	if (node->type == TOK_REDIRECT_OUT)
		return (red_out_overwrite(node->file_name));
	if (node->type == TOK_APPEND)
		return (red_out_append(node->file_name));
	if (node->type == TOK_REDIRECT_IN)
		return (red_in(node->file_name));
	if (node->type == TOK_HEREDOC)
	{
		hd = heredoc(node->file_name);
		if (hd == -1)
			return (1);
		if (dup2(hd, STDIN_FILENO) == -1)
			return (1);
		close(hd);
		return (0);
	}
	return (-1);
}

int	execute_builtins(t_ast *node, t_token_data **token_data)
{
	char	*value;
	int		len;

	value = node->args[0];
	len = ft_strlen(value);
	if (ft_strncmp(value, "cd", len) == 0)
		return (ft_cd(node->args, (*token_data)->env_list, (*token_data)->gc));
	else if (ft_strncmp(value, "echo", len) == 0 || ft_strncmp(value, "echo-n", len) == 0)
		return (ft_echo(node->args));
	else if (ft_strncmp(value, "pwd", len) == 0)
		return (ft_pwd(node->args));
	else if (ft_strncmp(value, "env", len) == 0)
		return (ft_env(node->args, (*token_data)->env_list));
	else if (ft_strncmp(value, "export", len) == 0)
		return (ft_export((*token_data)->env_list, node->args, (*token_data)->gc));
	else if (ft_strncmp(value, "unset", len) == 0)
		return (ft_unset((*token_data)->env_list, node->args));
	else if (ft_strncmp(value, "exit", len) == 0)
		return (ft_exit((*token_data)->gc));
	return (-1);
}

char	**env_to_array(t_env *top)
{
	size_t	count;
	t_env	*tmp;
	char	**out;
	int		i;

	count = 0;
	tmp = top;
	while (tmp && ++count)
		tmp = tmp->next;
	out = malloc(sizeof(char *) * (count + 1));
	if (!out)
		return (NULL);
	tmp = top;
	i = 0;
	while(tmp)
	{
        out[i] = malloc(ft_strlen(tmp->name) + ft_strlen(tmp->val) + 2);
        if (!out[i])
            return (NULL);
        ft_memcpy(out[i], tmp->name, ft_strlen(tmp->name));
        out[i][ft_strlen(tmp->name)] = '=';
        ft_memcpy(out[i] + ft_strlen(tmp->name) + 1, tmp->val, ft_strlen(tmp->val) + 1);
		tmp = tmp->next;
		i++;
    }
    out[count] = NULL;
    return (out);
}

/*
In the parent (interactive shell process) → use exit(status) when you run the builtin exit, because you want a clean shutdown.
In any forked child that will not call execve() successfully → use _exit(status) (or __exit, exit_group, etc.) so you leave immediately without touching user‑space buffers or handlers.
*/
//run inside *already‑forked* child (either from exec_pipe() or run_simple_cmd()).
void	exec_cmd(t_ast *node, int prev_read, t_token_data *td)
{
	t_token_data	*tmp;
	int				builtin_status;
	char			**envp;

	if (prev_read != -1)
	{
		dup2(prev_read, STDIN_FILENO);
		close(prev_read);
	}
	// if (apply_redirs(node))
	// 	_exit(1);
	tmp = td;
	builtin_status = execute_builtins(node, &tmp);
	if (builtin_status != -1)
		_exit(builtin_status);
	envp = env_to_array(td->env_list);
	execve(node->cmd_path, node->args, envp);
	perror("execve");
	_exit(127);
}

/*
** run_simple_cmd(): for commands that are NOT part of a pipeline.
** 1. Try built‑ins *in parent* (so cd/export really affect the shell state).
** 2. Otherwise fork, let the child call exec_cmd(), wait & return status.
*/
int run_simple_cmd(t_ast *node, t_token_data *td)
{
	t_token_data	*tmp;
	int				status;
	pid_t			pid;

	tmp = td;
	status = execute_builtins(node, &tmp);
	if (status != -1)
		return (status);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
		exec_cmd(node, -1, td);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}