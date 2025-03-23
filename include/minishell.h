#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

typedef enum s_tok_type
{
	TOK_PIPE,
	TOK_REDIRECT_IN,
	TOK_REDIRECT_OUT,
	TOK_APPEND,
	TOK_HEREDOC,
	TOK_OTHER,
	TOK_WORD_DQ,
	TOK_WORD,
	TOK_COMMAND,
	TOK_END
	// TOK_ENV
	// TOK_QUOTE,
	// TOK_AND,
	// TOK_OR
}					t_tok_type;

typedef struct s_token
{
	char			*value;
	t_tok_type		type;
	struct s_token	*prev;
	struct s_token	*next;
}					t_token;

typedef struct s_memory_track
{
}					t_memory_track;

// helper
void				start_message(void);
void				free_array(char **arr);
// parsing
int					modify_input(char *input, t_token **head);
int					tokenize(char **value, char *newinput, int words, t_token **head);
int					parsing_error(t_token **token_list);

#endif