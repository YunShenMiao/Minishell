NAME	= minishell
LDFLAGS = -L/lib/x86_64-linux-gnu -lreadline
CFLAGS = -I/usr/include/readline -Wextra -Wall -Werror 

SRC_DIR		= src
OBJ_DIR		= obj

SRC			= $(SRC_DIR)/test.c
OBJ			= $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)
# bonus

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(NAME): $(OBJ)
	@$(CC) $(OBJ) -o $(NAME) $(LDFLAGS) 

clean:
	rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: clean all

.PHONY: all clean fclean re