# Object and source path directories
OBJ_PATH = src/obj/
BUILTINS = src/builtins/
OBJ_PATH2 = $(BUILTINS)obj/
LIBFT_PATH = libft/

# Program & build names
NAME = minishell
LIBFT = $(LIBFT_PATH)libft.a

# Program sauce files
SRC = ./src/main.c src/env_utils.c src/utils.c src/executor.c src/expander.c src/signals.c src/tokenizer.c \
src/token_process.c src/cmd_table.c src/cleanup.c src/driver.c src/exec_utils.c src/malloc_tools.c src/apply_redir.c \
src/heredoc.c src/cmd_cleanup.c

SRC2 = ./$(BUILTINS)ft_cd.c $(BUILTINS)ft_echo.c $(BUILTINS)ft_env.c $(BUILTINS)ft_exit.c \
$(BUILTINS)ft_export.c $(BUILTINS)ft_pwd.c $(BUILTINS)ft_unset.c

# Object files
OBJ = $(SRC:src/%.c=$(OBJ_PATH)%.o)
OBJ2 = $(SRC2:$(BUILTINS)%.c=$(OBJ_PATH2)%.o)

# Compiler n flags
CC		=		cc
CFLAGS	= -Wall -Wextra -Werror -g -I.
LDFLAGS = -lreadline -L/opt/vagrant/embedded/lib/ -Iopt/vagrant/embedded/include/readline

# Color codes ✨
WHITE = \033[0;37m
BWHITE = \033[1;37m
BGREEN = \033[1;32m
RESET = \033[0m

# hide directory message (linux thing)
MAKEFLAGS += --no-print-directory

# Build magicc
all: $(NAME)

$(NAME): $(LIBFT) $(OBJ_PATH) $(OBJ_PATH2) $(OBJ) $(OBJ2)
	@echo "$(WHITE)Compiling $(BWHITE)$(NAME)$(WHITE) program...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJ) $(OBJ2) $(LIBFT) $(LDFLAGS) -o $(NAME)
	@echo "$(BWHITE)$(NAME)$(WHITE) program is $(BGREEN)ready! $(RESET)✅"

$(LIBFT):
	@echo "$(WHITE)Compiling $(BWHITE)libft$(WHITE) functions...$(RESET)"
	@make -C $(LIBFT_PATH) all
	@echo "$(WHITE)libft is $(BGREEN)ready! $(RESET)✅"

$(OBJ_PATH)%.o : src/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH2)%.o : $(BUILTINS)%.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)

$(OBJ_PATH2):
	@mkdir -p $(OBJ_PATH2)

clean:
	@rm -rf $(OBJ_PATH) $(OBJ_PATH2)
	@make clean -C $(LIBFT_PATH)
	@echo "$(BGREEN)cleaned like the blackhole you guys are getting if you DON'T GET TO COOKING$(WHITE)"

fclean: clean
	@rm -f $(NAME)
	@rm -f $(LIBFT)

re: fclean $(NAME)

# Complies and runs program at once (does not re)
runngun: all
	./minishell

# Calls the bitch of the subject to properly norm check the src folder so we don't need to switch directories
norm:
	cd src/ && norminette | grep Error

# Compiles and runs program and valgrind at once (and supresses readline leaks as it will always leak)
leak: all
	valgrind --leak-check=full --leak-resolution=high -s --track-origins=yes \
    --num-callers=500 --show-mismatched-frees=yes --show-leak-kinds=all \
    --track-fds=yes --trace-children=yes --gen-suppressions=no \
    --error-limit=no --undef-value-errors=yes --expensive-definedness-checks=yes \
    --read-var-info=yes --keep-debuginfo=yes \
    --suppressions=bin.supp --suppressions=readline.supp ./minishell

.PHONY: all clean fclean re leak runngun norm