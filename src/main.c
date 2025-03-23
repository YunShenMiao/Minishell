#include "../include/minishell.h"

// next: go deeper on terminal functions
// error strings for perror function
//builtins: echo with option -n -- cd with only a relative or absolute path
// pwd -- export -- unset -- env -- exit with no options

/* Interactive Mode:
- Display a prompt when the shell is waiting for commands.
- Signal Handling: need to catch SIGINT (for Ctrl-C), SIGQUIT (for Ctrl-\), but handle them in a way that only affects the prompt and process flow
- Implement a history feature where the user can press the up and down arrow keys to cycle through previously entered commands.
Non-Interactive Mode:
- The shell should execute commands from a file or input stream without prompting the user.
- no prompts should be displayed.
- Exit Status: Return the exit status of the last foreground process, using $?
- Handle input/output redirection (>, <, >>, <<) and pipes (|) the same way you would for interactive mode. */


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
			break;
		}
		if (*input)
			add_history(input);
		free(input);
	}
	return (0);
} */

int main()
{
	char *input;
	char *prompt = GREEN "minishell" BLUE ">" RESET " ";
	t_token	*token_list;

	start_message();
	while (1)
	{
		token_list = NULL;
		input = readline(prompt);
		if (!input)
		{
			printf("\n");
			break;
		}
		if (*input)
			add_history(input);
        modify_input(input, &token_list);
	}
    return(0);
}