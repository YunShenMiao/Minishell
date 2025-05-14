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
// print_ast((*token_data)->ast, 0, "Root: ");

// general structure function for parsing
int	parse_main(char *input, t_token_data **token_data, t_gc *gc, char **envp)
{
	char	*modified_input;

	init_token_data(input, token_data, gc, envp);
	if (modify_input(input, &modified_input, gc, token_data) == 1)
		return (1);
	(*token_data)->input = modified_input;
	if (tokenize(*token_data) == 1)
		return (1);
	if (build_ast(token_data) == NULL)
		return (1);
	if (expand_ast_nodes(token_data, &(*token_data)->ast) == 1)
		return (1);
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

static t_token_data	*init_and_setup(char **envp, char **prompt, t_gc **gc)
{
	t_token_data	*td;

	td = malloc(sizeof(t_token_data));
	if (!td)
		return (NULL);
	td->last_exit = 0;
	*gc = init_gc();
	handle_empty_envp(envp, gc);
	td->env_list = init_env(envp, *gc);
	td->gc = *gc;
	*prompt = GREEN "minishell" BLUE ">" RESET " ";
	// start_message();
	if (isatty(STDIN_FILENO))
	{
		disable_echoctl();
		setup_interactive_signals();
	}
	else
		setup_noninteractive_signals();
	return (td);
}

static void	input_loop(char *prompt, char **envp, t_token_data *td)
{
	char	*input;
	char	*line;

	while (1)
	{
		if (isatty(fileno(stdin)))
		{
			if (g_signal != 0)
			{
				reset_signal();
				input = readline(NULL);
				g_signal = 0;
			}
			else
				input = readline(prompt);
			if (!input)
			{
				td->last_exit = 0;
				break ;
			}
		}
		else
		{
			line = get_next_line(fileno(stdin));
			if (!line)
				break ;
			input = ft_strtrim(line, "\n");
			free(line);
		}
		if (!input)
			break ;
		if (ft_strlen(input) == 0 || empty_str(input) != 0)
		{
			free(input);
			continue ;
		}
		add_history(input);
		parse_execute(input, envp, &td);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_token_data	*td;
	t_gc			*gc;
	char			*prompt;
	int				le;

	if (argc < 1 || argv[0] == NULL)
		return (1);
	else if (argc > 1)
	{
		write(2, "🐢 minishell: input error: arguments not supported\n", 53);
		return(1);
	}
	td = init_and_setup(envp, &prompt, &gc);
	if (!td)
		return (1);
	input_loop(prompt, envp, td);
	le = td->last_exit;
	gc_free_all(gc, td->heredoc_id);
	free(td);
	rl_clear_history();
	exit(le);
}
