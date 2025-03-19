#ifndef MINISHELL_H
#define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>

typedef struct s_memory_track
{

}				t_memory_track;

//helper
void	start_message(void);
void	free_array(char **arr);

#endif