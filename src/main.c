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

#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"

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
	if (parse_main(input, token_data, (*token_data)->gc, envp) == 1)
	{
		gc_free_category((*token_data)->gc, TOKENS);
		gc_free_category((*token_data)->gc, PARSING);
		return;
	}
	if (check_empty_ast(*token_data) == 2)
	{
		gc_free_category((*token_data)->gc, TOKENS);
		gc_free_category((*token_data)->gc, PARSING);
		return;
	}
	handle_all_heredocs((*token_data)->ast, &((*token_data)->heredoc_id), (*token_data));
	exec_ast((*token_data)->ast, STDIN_FILENO, STDOUT_FILENO, (*token_data));
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
	if (isatty(STDIN_FILENO))
	{
		disable_echoctl();
		setup_interactive_signals();
	}
	else
		setup_noninteractive_signals();
	while (1)
	{
		// input = readline(prompt);
		// init_sig();
		// handle_signals();
		if (isatty(fileno(stdin)))
		{
			input = readline(prompt);
			if (!input)
				break;
		}
		else
		{
			char *line = get_next_line(fileno(stdin));
			if (!line)
				break;
			input = ft_strtrim(line, "\n"); //input is not freed here!!
			free(line);
		}
		if (!input || ft_strlen(input) == 0 || empty_str(input) != 0)
		{
			if (input)
				free(input);
			continue ;
		}
		else
		{
			// if (g_signal == SIGINT)
			// {
			// 	token_data->last_exit = 1; // should be 1 here, but somehow not showing
			// 	// g_signal = 0;
			// 	// free(input);
			// 	continue;
			// }
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
