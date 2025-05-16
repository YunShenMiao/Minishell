/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 10:16:27 by xueyang           #+#    #+#             */
/*   Updated: 2025/05/15 16:52:00 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/**************************************************************************/
/*							INCLUDES&DEFINES						      */
/**************************************************************************/

# include "../libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>
# include <dirent.h>
# include <signal.h>
# include <termios.h>

# define GREEN "\033[1;32m"
# define BLUE "\033[1;34m"
# define RESET "\033[0m"

extern volatile sig_atomic_t	g_signal;
/**************************************************************************/
/*							GARBAGE_COLLECTOR_STRUCTS				      */
/**************************************************************************/

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
	t_garbage	*gc_list[MEM_CAT];
}						t_gc;

/**************************************************************************/
/*								ENUMS					  				  */
/**************************************************************************/

typedef enum s_error
{
	INVALID_COMMAND,
	SYNTAX_ERROR,
	UNCLOSED_QUOTES,
	IS_DIR
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
	TOK_WORD,
	TOK_FILE,
	TOK_END
}					t_tok_type;

/**************************************************************************/
/*								STRUCTS					  				  */
/**************************************************************************/

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
	int				quote;
	int				hd;
	struct s_ast	*left;
	struct s_ast	*right;
	char			*cmd_path;
	t_gc			*gc;
}					t_ast;

typedef struct s_token_data
{
	char			*input;
	t_token			*token_list;
	t_ast			*ast;
	int				sq;
	int				dq;
	int				start;
	int				end;
	int				finish;
	int				syntax_error;
	char			**envp;
	char			*expand_str;
	struct s_env	*env_list;
	t_gc			*gc;
	int				last_exit;
	int				heredoc_id;
	int				in_pipeline;
	int				env_cmd;
	int				hd;
}				t_token_data;

typedef struct s_redi_ctx
{
	int				*in_fd;
	int				*out_fd;
	t_ast			**cmd_node;
	t_token_data	*td;
}	t_redi_ctx;

/**************************************************************************/
/*							ENVIRONMENT VARIABLES				  		  */
/**************************************************************************/

typedef struct s_env
{
	char			*name;
	char			*val;
	struct s_env	*next;
}					t_env;

typedef struct s_data
{
	t_token	*head;
	t_token	*cur_token;
	t_env	*top_env;
}			t_data;

/**************************************************************************/
/*							FUNCTIONS							  		  */
/**************************************************************************/

// tokens //
int		modify_input(char *input, char **modified_input, t_gc *gc,
			t_token_data **token_data);
int		tokenize(t_token_data *token_data);
int		init_token_data(char *input, t_token_data **token_data,
			t_gc *gc, char **envp);

// parsing //
t_ast	*build_ast(t_token_data **token_data);
t_ast	*create_ast_node(t_token_data **token_data, t_tok_type type);
t_ast	*parse_redirections(t_token_data **token_data, t_token **current,
			t_ast *prev);
t_ast	*parse_command(t_token_data **token_data, t_token **current);
t_ast	*parse_pipes(t_token_data **token_data, t_token **current);
char	*find_path(char *cmd, t_env *top, t_gc *gc);
int		quote_status(t_token_data **token_data, char input);
int		expand_ast_nodes(t_token_data **token_data, t_ast **ast);
char	*handle_quotes(t_token_data **token_data, char **str);
int		valid_cmd(t_token_data **token_data, t_ast *node);
void	parse_command_args(t_token_data **token_data, t_token **current,
			t_ast	*cmd_node);
void	expand_var(t_token_data **token_data, int *i, int *count, char *new);
t_ast	*add_cmd(t_token_data *token_data, t_tok_type type);

// builtins
int		ft_echo(char **args);
int		ft_pwd(char **args);
int		ft_exit(char **args, t_token_data *td);
int		ft_cd(char **args, t_env *top, t_gc *gc);
int		ft_env(char **args, t_env *top_env);
int		ft_export(t_env	*top_env, char **args, t_token_data *td);
int		ft_unset(t_env	*top_env, char **args);
int		is_numeric(char *str);
int		shell_level(t_token_data *td);
char	**convert_to_envp(t_env *env_list, t_gc *gc);

//cd utils
char	*find_home(t_env *top);
int		update_pwds(t_env *top, char *new_pwd, t_gc *gc);
int		swap_pwds(t_env *top, t_gc *gc);
int		find_last_slash(char *path);
char	*get_parent_dir(char *cur_dir, t_gc *gc);
char	*normalize_path(const char *path, t_gc *gc);

//execution
int		execute_builtins(t_ast *node, t_token_data *token_data);
int		is_builtin(char **args);
int		exec_ast(t_ast *node, int input_fd, int output_fd, t_token_data *td);
int		setup_redi(int input_fd, int output_fd, int *s_stdin, int *s_stdout);
void	restore_stdio(int saved_stdin, int saved_stdout);
int		resolve_redirections(t_ast *node, t_redi_ctx *ctx);
int		exec_pipe(t_ast *node, int input_fd, int output_fd, t_token_data *td);

//redirections
int		red_out_overwrite(char *filename);
int		red_out_append(char *filename);
int		red_in(char *filename);
void	handle_all_heredocs(t_ast *node, int *heredoc_id, t_token_data *td);
int		write_to_file(t_ast *node, char *file, t_token_data *td);
void	cleanup_heredoc_tempfiles(int max_id);
char	*expand_heredoc(char *line, t_env *env_list, int last_exit);

// heredoc utils
void	ft_itoa_simple(int n, char *buf);
char	*generate_heredoc_filename(t_gc *gc, int id);
int		process_heredoc_line(int fd, char *line, t_ast *node, t_token_data *td);
char	*heredoc_readline(const char *prompt);
void	cleanup_heredoc_tempfiles(int max_id);

// signals
void	setup_interactive_signals(void);
void	setup_noninteractive_signals(void);
void	disable_echoctl(void);
void	reset_signal(void);
void	set_signal_heredoc(void);

/**************************************************************************/
/*							HELPER-FUNCTIONS					  		  */
/**************************************************************************/

//debug_prints
void	print_ast(t_ast *node, int depth, char *pos);
// helper
void	start_message(void);
void	free_array(char **arr);
int		ft_put_to_fd(char *str, int fd);
int		ft_ministrcmp(const char *s1, const char *s2);
// helper2
int		empty_str(char *str);
int		check_empty_ast(t_token_data *token_data);
void	handle_empty_envp(char **envp, t_gc **gc);
// gc_libft
char	*ft_env_substr(char const *s, unsigned int start, size_t len, t_gc *gc);
char	*ft_env_strdup(const char *src, t_gc *gc, t_mem_location loc);
char	*ft_env_strjoin(char const *s1, char const *s2, t_gc *gc);
char	*ft_strndup(t_gc *gc, const char *src, int start, int end);
// error_handling
int		error_general(char *msg);
void	ft_perror_parsing(t_token_data **token_data, int error_id,
			char *error_info);
//env_list
t_env	*create_env(char *name, char *value, t_gc *gc);
t_env	*ft_env_last(t_env *lst);
void	ft_env_add_back(t_env **lst, t_env *new);
void	ft_env_del(t_env **top, t_env *to_del);
t_env	*search_name_node(t_env *top, char *name);
char	*search_name_val(t_env *top, char *name);
int		update_env_var(t_env **lst, char *name, char *new_val, t_gc *gc);
//init_envp
int		count_arr_row(char **arr);
int		find_sign(char *str, char c);
// int		init_env(char **envp, t_gc *gc);
t_env	*init_env(char **envp, t_gc *gc);
// garbage collector
t_gc	*init_gc(void);
void	*gc_malloc(t_gc *gc, t_mem_location category, unsigned long size);
void	gc_free_category(t_gc *gc, t_mem_location category);
void	gc_free_all(t_gc *gc, int heredoc_id);

#endif