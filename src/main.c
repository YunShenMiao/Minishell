#include "include/minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

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


int is_interactive()
{
    printf("%d\n", isatty(STDIN_FILENO));
    return(isatty(STDIN_FILENO));
}

void interactive_shell()
{
    char *line;
    int status;

    status = 1;
    while(status)
    {
    line = readline("minishell$ ");
    if (line == NULL)
    break;
    add_history(line);
    /* split_line(line); */
    printf("%s\n", line);
    }
}

void non_interactive_shell()
{
    printf("not interactive\n");
}

int main()
{
    start_message();
    if (is_interactive())
    interactive_shell();
    else
    non_interactive_shell();
    return(0);
}