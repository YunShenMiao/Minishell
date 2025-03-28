#include "../include/minishell.h"

// error strings for perror function

#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"


/* int main(void)
{
	char *input;
	char *prompt = GREEN "minishell" BLUE ">" RESET " ";

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
		free(input);
	}
	return (0);
} */

// just to check tokenization
void	print_list(t_token *head)
{
	t_token	*current;
	int		i;

	current = head;
	i = 1;
	printf("Doubly Linked List: \n");
	while (current != NULL)
	{
		printf("%d: %s, type: %u\n", i, current->value, current->type);
		current = current->next;
		i++;
	}
}

//general structure function for parsing
int	parse_main(char *input, t_token_data **token_data, t_gc *gc)
{
	char *modified_input;

	if (modify_input(input, &modified_input, gc) == 1)
		return (printf("Allocation Error\n"), 1);
	if (init_token_data(modified_input, token_data, gc) == 1)
		return (printf("Allocation Error\n"), 1);
	if (tokenize(token_data) == 1)
		return (1);
	if (syntax_error(token_data) == 1)
		return(1);
	print_list((*token_data)->token_list);
	gc_free_all(gc);
	return (0);
}

int	main(void)
{
	char *input;
	char *prompt = GREEN "minishell" BLUE ">" RESET " ";
	t_token_data *token_data;
	t_gc *gc;

	start_message();
	while (1)
	{
		token_data = NULL;
		input = readline(prompt);
		if (!input)
		{
			printf("\n");
			break ;
		}
		if (*input)
			add_history(input);
		gc = init_gc();
		if (ft_strncmp(input, "exit", 4) == 0)
		{
			free(input);
			gc_free_all(gc);
		break;
		}
		if (parse_main(input, &token_data, gc) == 1)
		gc_free_all(gc);
	}
	return (0);
}
