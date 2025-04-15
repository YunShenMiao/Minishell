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



/****************************************************************************************************/
/*									GARBAGE_COLLECTOR_STRUCTS										*/
/****************************************************************************************************/

typedef enum s_mem_location
{
	TOKENS,
	PARSING,
	BUILT_IN,
	EXECUTION,
	ENV,
	MEM_CAT
}					t_mem_location;

typedef struct s_garbage
{
	void				*ptr;
	struct s_garbage	*next;
}					t_garbage;

typedef struct s_gc
{
	t_garbage *gc_list[MEM_CAT];
}						t_gc;

/****************************************************************************************************/
/*											TOKEN_STRUCTS											*/
/****************************************************************************************************/

typedef enum s_error
{
	INVALID_COMMAND,
	SYNTAX_ERROR,
	UNCLOSED_QUOTES

}				t_error;

typedef enum s_tok_type
{
	TOK_INVALID = -1,
	TOK_PIPE,
	TOK_REDIRECT_IN,
	TOK_REDIRECT_OUT,
	TOK_APPEND,
	TOK_HEREDOC,
	TOK_COMMAND,
	TOK_WORD_DQ,
	TOK_WORD_SQ,
	TOK_WORD_NQ,
	TOK_EMPTY_WORD,
	TOK_END
	// TOK_ENV
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

typedef struct s_ast
{
	t_tok_type		type;
	char			**args;
	char			*file_name;
	int				fd;
	struct s_ast	*left;
	struct s_ast	*right;
	char			*cmd_path;
	t_gc			*gc;
}					t_ast;

typedef struct s_token_data
{
	char		*input;
	t_token		*token_list;
	t_ast		*ast;
	struct s_env env_list;
	int			in_SQ;
	int			in_DQ;
	int			start;
	int			end;
	int			first;
	int			finish;
	int			syntax_error;
	char		**envp;
	t_gc		*gc;
}				t_token_data;

/****************************************************************************************************/
/*									ENVIRONMENT VARIABLES											*/
/****************************************************************************************************/

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

// error_handling
int		error_general(char *msg);
int		error_free(char *msg, t_data *shell);

//env_list
t_env	*create_env(char *name, char *value, t_gc *gc);
t_env	*ft_env_last(t_env *lst);
void	ft_env_add_back(t_env **lst, t_env *new);
void	ft_env_del(t_env *top, t_env *to_del);
t_env	*search_name_node(t_env **lst, char *name);
char	*search_name_val(t_env **lst, char *name);
int		update_env_var(t_env **lst, char *name, char *new_val, t_gc *gc);

//init_envp
int		count_arr_row(char **arr);
int		find_sign(char *str, char c);
int		init_env(char **envp, t_gc *gc);

// garbage collector
t_gc    *init_gc(void);
void 	*gc_malloc(t_gc *gc, t_mem_location category, unsigned long size);
void	gc_free_category(t_gc *gc, t_mem_location category);
void	gc_free_all(t_gc *gc);

// tokens
int		modify_input(char *input, char **modified_input, t_gc *gc);
int 	tokenize(t_token_data **token_data);
int 	init_token_data(char *input, t_token_data **token_data, t_gc *gc, char **envp);
int		add_token(t_token_data **token_data);
int		token_command(char *value, size_t len);

// parsing
void 	ft_perror_parsing(int error_id, char* error_info);
t_ast 	*build_ast(t_token_data **token_data);
char	*ft_strndup(t_gc *gc, const char *src, int start, int end);
t_ast 	*create_ast_node(t_token_data **token_data, t_tok_type type);
int		is_word(t_tok_type type);
t_ast	*parse_redirections(t_token_data **token_data, t_token **current);
t_ast	*parse_command(t_token_data **token_data, t_token **current);
t_ast	*parse_pipes(t_token_data **token_data, t_token **current);
char	*find_path(char *cmd, char **envp);
void	quote_status(t_token_data **token_data, char input);
int 	expand_ast_nodes(t_token_data **token_data, t_ast **ast);
char	*handle_quotes(t_token_data **token_data, char **str);

// builtins
int		ft_echo(t_token *current, t_env *top_env);
int		ft_pwd(t_token *current);
int		ft_exit(t_token *current);
int		ft_cd(t_token *current, t_env *top);
int		ft_env(t_env *top_env, t_token *current);
int		ft_export(t_env	*top_env, t_token *current, t_gc *gc);
int		ft_unset(t_env	*top_env, t_token *current);

#endif