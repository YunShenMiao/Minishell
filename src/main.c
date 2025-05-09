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

// #define GREEN "\033[1;32m"
// #define BLUE "\033[1;34m"
// #define RESET "\033[0m"

// general structure function for parsing
// print_list((*token_data)->token_list);
// test(token_data, envp);
int	parse_main(char *input, t_token_data **token_data, t_gc *gc, char **envp)
{
	char	*modified_input;

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
	print_ast((*token_data)->ast, 0, "Root: ");
	return (0);
}

void	parse_execute(char *input, char **envp, t_token_data **td)
{
	if (parse_main(input, td, (*td)->gc, envp) == 1)
	{
		gc_free_category((*td)->gc, TOKENS);
		gc_free_category((*td)->gc, PARSING);
		return ;
	}
	if (check_empty_ast(*td) == 2)
	{
		gc_free_category((*td)->gc, TOKENS);
		gc_free_category((*td)->gc, PARSING);
		return ;
	}
	handle_all_heredocs((*td)->ast, &((*td)->heredoc_id), (*td));
	exec_ast((*td)->ast, STDIN_FILENO, STDOUT_FILENO, (*td));
	gc_free_category((*td)->gc, TOKENS);
	gc_free_category((*td)->gc, PARSING);
}

// if string only space or tab -> just new prompt
int	main(int argc, char **argv, char **envp)
{
	char			*input;
	char			*prompt;
	t_token_data	*token_data;
	t_gc			*gc;
	int				le;
	char			*line;

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
		if (isatty(fileno(stdin)))
		{
			input = readline(prompt);
			if (!input)
				break ;
		}
		else
		{
			line = get_next_line(fileno(stdin));
			if (!line)
				break ;
			input = ft_strtrim(line, "\n");
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
