NAME	= minishell
LDFLAGS = -lreadline -Llibft -lft

CFLAGS = -I/usr/include/readline -Ilibft -Wextra -Wall -Werror


SRC_DIR		= src
OBJ_DIR		= obj

SRC			= 	$(SRC_DIR)/main.c \
				$(SRC_DIR)/utils/helper.c $(SRC_DIR)/utils/garbage_collector.c \
				$(SRC_DIR)/tokens/init_token_data.c $(SRC_DIR)/tokens/modify_input.c \
				$(SRC_DIR)/tokens/token_list.c $(SRC_DIR)/tokens/tokenize.c \
				$(SRC_DIR)/parsing/parsing_utils.c $(SRC_DIR)/parsing/ast.c \
				$(SRC_DIR)/parsing/ast_redirect.c $(SRC_DIR)/parsing/ast_command.c \
				$(SRC_DIR)/parsing/ast_pipes.c $(SRC_DIR)/parsing/path.c
OBJ			= $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

# $(SRC_DIR)/builtins/cd.c $(SRC_DIR)/builtins/echo.c $(SRC_DIR)/builtins/env.c \
# $(SRC_DIR)/builtins/exit.c $(SRC_DIR)/builtins/export.c $(SRC_DIR)/builtins/pwd.c \
# $(SRC_DIR)/builtins/unset.c \
# $(SRC_DIR)/execution/exe.c $(SRC_DIR)/execution/path.c $(SRC_DIR)/execution/pipe.c \
# $(SRC_DIR)/execution/redirection.c $(SRC_DIR)/execution/signal.c


all: $(OBJ_DiR) libft $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ): $(SRC)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS) 

libft:
	@make -C libft

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)
	@make -C libft fclean

re: fclean all

.PHONY: all clean fclean re libft