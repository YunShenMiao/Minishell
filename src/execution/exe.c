/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 20:06:50 by xueyang           #+#    #+#             */
/*   Updated: 2025/04/21 16:43:09 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	do_redir(t_tok_type tp, t_ast *node)
{
	int	hd;

	if (tp == TOK_REDIRECT_OUT)
		return (red_out_overwrite(node->file_name));
	if (tp == TOK_APPEND)
		return (red_out_append(node->file_name));
	if (tp == TOK_REDIRECT_IN)
		return (red_in(node->file_name));
	if (tp == TOK_HEREDOC)
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

int	exec_redir_normal(t_ast *node, t_token_data **td, t_tok_type tp)
{
	if (node->left)
		exec_redir_normal(node->left, td, node->type);
	if (node->right)
		exec_redir_normal(node->right, td, node->type);
	if (node->type == TOK_COMMAND)
		run_simple_cmd(node, *td);
	if (node->type == TOK_FILE)
		return (do_redir(tp, node));
	return (-1);
}

int	exec_redir_pipe(t_ast *node, t_token_data **td, t_tok_type tp, int prev_read)
{
	if (node->left)
		exec_redir_pipe(node->left, td, node->type, prev_read);
	if (node->right)
		exec_redir_pipe(node->right, td, node->type, prev_read);
	if (node->type == TOK_COMMAND)
		exec_cmd(node, prev_read, *td);
	if (node->type == TOK_FILE)
		return (do_redir(tp, node));
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
		return (ft_exit(node->args, (*token_data)->gc));
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

// run_simple_cmd(): for commands that are NOT part of a pipeline.
int	run_simple_cmd(t_ast *node, t_token_data *td)
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
	if (waitpid(pid, &status, 0) == -1)
		perror("waitpid");
	if (WIFEXITED(status))
		td->last_exit = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		td->last_exit = 128 + WTERMSIG(status);
	return (td->last_exit);
}