# Object and source path directories
OBJ_PATH = obj/
LIBFT_PATH = libft/

# Program & build names
NAME = minishell
LIBFT = $(LIBFT_PATH)libft.a

# Program sauce files
SRC = main.c signals.c

# Object files
OBJ = $(SRC:%.c=$(OBJ_PATH)%.o)

# Compiler n flags
CC		=		cc
CFLAGS	= -Wall -Werror -Wextra
LDFLAGS = -lreadline

# Color codes ✨
WHITE = \033[0;37m
BWHITE = \033[1;37m
BGREEN = \033[1;32m
RESET = \033[0m

# hide directory message (linux thing)
MAKEFLAGS += --no-print-directory

# Build magicc
all: $(NAME)

$(NAME): $(LIBFT) $(OBJ_PATH) $(OBJ)
	@echo "$(WHITE)Compiling $(BWHITE)$(NAME)$(WHITE) program...$(RESET)"
	$(CC) $(CFLAGS) $(LIBFT) $(OBJ) -lreadline -o $(NAME)
	@echo "$(BWHITE)$(NAME)$(WHITE) program is $(BGREEN)ready! $(RESET)✅"

$(LIBFT):
	@echo "$(WHITE)Compiling $(BWHITE)libft$(WHITE) functions...$(RESET)"
	@make -C $(LIBFT_PATH) all
	@echo "$(WHITE)libft is $(BGREEN)ready! $(RESET)✅"

$(OBJ_PATH)%.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)

clean:
	@rm -rf $(OBJ_PATH)
	@make clean -C $(LIBFT_PATH)
	@echo "$(BGREEN)cleaned like the blackhole you guys are getting if you DON'T GET TO COOKING$(WHITE)"

fclean: clean
	@rm -f $(NAME)
	@rm -f $(LIBFT)

re: fclean $(NAME)

.PHONY: all clean fclean re