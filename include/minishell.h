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
	TOK_INVALID = -1,
	TOK_PIPE,
	TOK_REDIRECT_IN,
	TOK_REDIRECT_OUT,
	TOK_APPEND,
	TOK_HEREDOC,
	TOK_EMPTY_WORD,
	TOK_WORD_DQ,
	TOK_WORD_SQ,
	TOK_WORD_NQ,
	TOK_COMMAND,
	TOK_END
	// TOK_ENV
	// TOK_AND,
	// TOK_OR
}					t_tok_type;

// starting structure for garbage collector
// typedef enum s_mem_location
// {
// 	TOKEN,
// 	PARSING,
// 	BUILT_IN,
// 	EXECUTION
// }					t_mem_location;

// typedef struct s_garbage
// {
// 	void				*ptr;
// 	t_mem_location 		loc;
// 	struct s_garbage	*next;
// }					t_garbage;

typedef struct s_token
{
	char			*value;
	t_tok_type		type;
	struct s_token	*prev;
	struct s_token	*next;
}					t_token;

typedef struct s_token_data
{
	char		*input;
	t_token		*token_list;
	int			in_SQ;
	int			in_DQ;
	int			start;
	int			end;
	int			first;
	// t_garbage	*gc;
}				t_token_data;

typedef struct s_env
{
	char			*name;
	char			*val;
	struct t_env	*next;
}					t_env;

typedef struct s_data
{
	t_token	*head;
	t_token	*cur_token;
	t_env	*top_env;
}			t_data;

// helper
void	start_message(void);
void	free_array(char **arr);
int		ft_put_to_fd(char *str, int fd);
t_env	*create_env(char *name, char *value);
t_env	*ft_env_last(t_env *lst);
void	ft_env_add_back(t_env **lst, t_env *new);
int		error_general(char *msg);
int		error_free(char *msg, t_data *shell);
int		count_arr_row(char **arr);
int		find_equal(char *str);
int		init_env(char **envp);
t_env	*search_name_node(t_env **lst, char *name);
char	*search_name_val(t_env **lst, char *name);
int		update_env_var(t_env **lst, char *name, char *new_val);

// parsing
int		modify_input(char *input, char **modified_input);
int 	tokenize(t_token_data **token_data);
int		parsing_error(t_token **token_list);
int 	init_token_data(char *input, t_token_data **token_data);
int		add_token(t_token_data **token_data);

// builtins
int		ft_echo(t_token *current);
int		ft_pwd(t_token *current);
int		ft_exit(t_token *current);
int		ft_cd(t_token *current);

#endif