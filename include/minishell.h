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

# define GREEN "\033[1;32m"
# define BLUE "\033[1;34m"
# define RESET "\033[0m"

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

typedef struct s_env
{
	char			*name;
	char			*val;
	struct t_env	*next;
}					t_env;

typedef struct s_data
{
	t_token	*head;
	t_env	*top_env;
}			t_data;

typedef struct s_memory_track
{
}					t_memory_track;

// helper
void	start_message(void);
void	free_array(char **arr);
int		ft_put_to_fd(char *str, int fd);
t_env	*create_env(char *name, char *value);
t_env	*ft_env_last(t_env *lst);
void	ft_env_add_back(t_env **lst, t_env *new);

// parsing
int		modify_input(char *input);
int		tokenize(char **value, char *newinput, int words);

// builtins
int		ft_echo(t_token *current);
int		ft_pwd(t_token *current);
int		ft_exit(t_token *current);
int		ft_cd(t_token *current);

#endif