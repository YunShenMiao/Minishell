/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 18:09:52 by jwardeng          #+#    #+#             */
/*   Updated: 2025/04/16 18:09:52 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// error strings for perror function

#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"


int	execute_builtins(t_ast *node, t_token_data **token_data)
{
	char *value;
	int len;
	int result;

	value = node->args[0];
	len = ft_strlen(node->args[0]);
	result = -1;
	if (ft_strncmp(value, "cd", len) == 0)
		result = ft_cd(node->args, (*token_data)->env_list, (*token_data)->gc);
	else if (ft_strncmp(value, "echo", len) == 0 || ft_strncmp(value, "echo-n",
			len) == 0)
		result = ft_echo(node->args);
	else if (ft_strncmp(value, "pwd", len) == 0)
		result = ft_pwd(node->args);
	else if (ft_strncmp(value, "env", len) == 0)
		result = ft_env(node->args, (*token_data)->env_list);
	else if (ft_strncmp(value, "export", len) == 0)
		result = ft_export((*token_data)->env_list, node->args,
				(*token_data)->gc);
	if (ft_strncmp(value, "unset", len) == 0)
		result = ft_unset((*token_data)->env_list, node->args);
	else if (ft_strncmp(value, "exit", len) == 0)
		result = ft_exit((*token_data)->gc);
	return (result);
}

void	execute_cmd(t_ast *node, char **envp)
{
	pid_t pid;

	pid = fork();
	if (pid == 0)
	{
		execve(node->cmd_path, node->args, envp);
		perror("execve failed");
		exit(1);
	}
	else
	{
		waitpid(pid, NULL, 0);
	}
}

// int	exec_redirect(t_ast *node)
// {
// 	if (node->type == TOK_APPEND)
// 	{
// 		red_out_append(node->file_name);
// 	}
// 	else if (node->type == TOK_HEREDOC)
// 		heredoc();
// 	else if (node->type == TOK_REDIRECT_IN)
// 		red_in_overwrite(node->file_name);
// 	else if (node->type == TOK_REDIRECT_OUT)
// 	{
// 		red_out_overwrite(node->filename);
// 	}
// }

//
	// either split into if pipes execute everything in pipes and no pipes separate
int	execution_main(t_token_data **token_data, t_ast *node)
{
	if (node->type == TOK_PIPE)
	{
		printf("pipe\n");
		/* exec_pipe(); */
		return (0);
	}
	else if (node->type == TOK_COMMAND)
	{
		printf("cmd\n");
		if (execute_builtins(node, token_data) == -1)
			execute_cmd(node, (*token_data)->envp);
	}
	else if (node->type == TOK_APPEND || node->type == TOK_HEREDOC
		|| node->type == TOK_REDIRECT_IN || node->type == TOK_REDIRECT_OUT)
	{
		printf("redirect\n");
		/* exec_redirect(node); */
	}
	if (node->left != NULL)
		execution_main(token_data, node->left);
	if (node->right != NULL)
		execution_main(token_data, node->right);
	return (0);
}

void	print_ast(t_ast *node, int depth, char *pos)
{
	int i;

	if (!node)
		return ;
	i = 0;
	while (i < depth)
	{
		printf("  ");
		i++;
	}
	printf("%s ", pos);
	if (node->type == TOK_COMMAND)
	{
		printf("CMD: ");
		i = 0;
		while (node->args[i])
		{
			printf("%s ", node->args[i]);
			i++;
		}
	}
	else if (node->type == TOK_PIPE)
		printf("PIPE");
	else if (node->type == TOK_REDIRECT_IN || node->type == TOK_REDIRECT_OUT
		|| node->type == TOK_APPEND || node->type == TOK_HEREDOC)
		printf("REDIR: %d (fd=%d) -> %s", node->type, node->fd,
			node->file_name);
	printf("\n");

	print_ast(node->left, depth + 1, "Left:");
	print_ast(node->right, depth + 1, "Right:");
}

// general structure function for parsing
int	parse_main(char *input, t_token_data **token_data, t_gc *gc, char **envp)
{
	char *modified_input;

	if (modify_input(input, &modified_input, gc) == 1)
		return (printf("Allocation Error\n"), 1);
	if (init_token_data(modified_input, token_data, gc, envp) == 1)
		return (printf("Allocation Error\n"), 1);
	if (tokenize(*token_data) == 1)
		return (1);
	if (build_ast(token_data) == NULL)
		return (1);
	if (expand_ast_nodes(token_data, &(*token_data)->ast) == 1)
		return (1);
	// print_list((*token_data)->token_list);
	print_ast((*token_data)->ast, 0, "Root: ");
	// test(token_data, envp);
	return (0);
}

void	parse_execute(char *input, char **envp)
{
	t_token_data *token_data;
	t_gc *gc;

	token_data = NULL;
	gc = init_gc();

	if (parse_main(input, &token_data, gc, envp) == 1)
		gc_free_all(gc);
	else
	{
		if (execution_main(&token_data, token_data->ast) == 1)
			gc_free_all(gc);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char *input;
	char *prompt;

	if (argc < 1 || argv[0] == NULL)
		return (1);
	prompt = GREEN "minishell" BLUE ">" RESET " ";
	start_message();
	while (1)
	{
		input = readline(prompt);
		if (!input)
		{
			printf("\n");
			break ;
		}
		if (*input)
			add_history(input);
		parse_execute(input, envp);
	}
	return (0);
}
