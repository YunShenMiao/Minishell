/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2025/04/16 18:09:52 by jwardeng          #+#    #+#             */
/*   Updated: 2025/04/16 18:09:52 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// error strings for perror function

#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"

volatile sig_atomic_t glsignal = 0;

// int	execute_builtins(t_ast *node, t_token_data **token_data)
// {
// 	char *value;
// 	int len;
// 	int result;

// 	value = node->args[0];
// 	len = ft_strlen(node->args[0]);
// 	result = -1;
// 	if (ft_strncmp(value, "cd", len) == 0)
// 		result = ft_cd(node->args, (*token_data)->env_list, (*token_data)->gc);
// 	else if (ft_strncmp(value, "echo", len) == 0 || ft_strncmp(value, "echo-n",
// 			len) == 0)
// 		result = ft_echo(node->args);
// 	else if (ft_strncmp(value, "pwd", len) == 0)
// 		result = ft_pwd(node->args);
// 	else if (ft_strncmp(value, "env", len) == 0)
// 		result = ft_env(node->args, (*token_data)->env_list);
// 	else if (ft_strncmp(value, "export", len) == 0)
// 		result = ft_export((*token_data)->env_list, node->args,
// 				(*token_data)->gc);
// 	if (ft_strncmp(value, "unset", len) == 0)
// 		result = ft_unset((*token_data)->env_list, node->args);
// 	else if (ft_strncmp(value, "exit", len) == 0)
// 		result = ft_exit((*token_data)->gc);
// 	return (result);
// }

// void	execute_cmd(t_ast *node, char **envp)
// {
// 	pid_t pid;

// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		execve(node->cmd_path, node->args, envp);
// 		perror("execve failed");
// 		exit(1);
// 	}
// 	else
// 	{
// 		waitpid(pid, NULL, 0);
// 	}
// }

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
		// printf("pipe\n");
		return (exec_pipe(node, -1, (*token_data)));
	}
	else if (node->type == TOK_COMMAND)
	{
		// printf("cmd\n");
		return (run_simple_cmd(node, (*token_data)));
	}
	else if (node->type == TOK_APPEND || node->type == TOK_HEREDOC
		|| node->type == TOK_REDIRECT_IN || node->type == TOK_REDIRECT_OUT)
	{
		// printf("redirect\n");
		return (exec_redir_normal(node, token_data, node->type));
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
		printf("REDIR: %d -> %s", node->type, node->file_name);
	else if (node->type == TOK_FILE)
		printf("FILE: %s", node->file_name);
	printf("\n");

	print_ast(node->left, depth + 1, "Left:");
	print_ast(node->right, depth + 1, "Right:");
}

// general structure function for parsing
int	parse_main(char *input, t_token_data **token_data, t_gc *gc, char **envp)
{
	char *modified_input;

	if (init_token_data(input, token_data, gc, envp) == 1)
		return (printf("Allocation Error\n"), 1);
	if (modify_input(input, &modified_input, gc, token_data) == 1)
		return (printf("Allocation Error\n"), 1);
	(*token_data)->input = modified_input;
	if (tokenize(*token_data) == 1)
		return (1);
	if (build_ast(token_data) == NULL)
		return (1);
	if (expand_ast_nodes(token_data, &(*token_data)->ast) == 1)
		return (1);
	// print_list((*token_data)->token_list);
	// print_ast((*token_data)->ast, 0, "Root: ");
	// test(token_data, envp);
	return (0);
}

void	parse_execute(char *input, char **envp, t_token_data **token_data)
{
	// t_token_data *token_data;
	// t_gc *gc;

	// token_data = NULL;
	// gc = init_gc();

	// parse_main(input, token_data, (*token_data)->gc, envp);
	// execution_main(token_data, (*token_data)->ast);
	if (parse_main(input, token_data, (*token_data)->gc, envp) == 1)
	{
		gc_free_category((*token_data)->gc, TOKENS);
		gc_free_category((*token_data)->gc, PARSING);
		return;
	}
		// gc_free_all((*token_data)->gc);
	// printf("value: %s\n", search_name_val((*token_data)->env_list, "PWD"));
	handle_all_heredocs((*token_data)->ast, &((*token_data)->heredoc_id), (*token_data));
	// execution_main(token_data, (*token_data)->ast);
	exec_ast((*token_data)->ast, STDIN_FILENO, STDOUT_FILENO, (*token_data));
		// gc_free_all((*token_data)->gc);
		gc_free_category((*token_data)->gc, TOKENS);
		gc_free_category((*token_data)->gc, PARSING);
}

// if string only space or tab -> just new prompt
int	main(int argc, char **argv, char **envp)
{
	char *input;
	char *prompt;
	t_token_data *token_data;
	t_gc *gc;
	int le;

	token_data = malloc(sizeof(t_token_data));
	if (!token_data)
		return (1);
	token_data->last_exit = 0;
	gc = init_gc();
	token_data->env_list = init_env(envp, gc);
	if (argc < 1 || argv[0] == NULL)
		return (1);
	prompt = GREEN "minishell" BLUE ">" RESET " ";
	// start_message();
	while (1)
	{
		// input = readline(prompt);
		// init_sig();
		// handle_signals();
		if (isatty(fileno(stdin)))
		{
			input = readline(prompt);
		}
		else
		{
			char *line = get_next_line(fileno(stdin));
			if (!line)
				break;
			input = ft_strtrim(line, "\n"); //input is not freed here!!
			free(line);
		}
		if (!input || ft_strlen(input) == 0)
			continue ;
		else
		{
			token_data->gc = gc;
			add_history(input);
			parse_execute(input, envp, &token_data);
			// gc = init_gc();
		}
	}
	le = (token_data->last_exit);
	free(token_data);
	if (gc)
	free(gc);
	exit(le);
	return (0);
}
