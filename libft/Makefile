# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marvin <marvin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/17 21:00:03 by nanasser          #+#    #+#              #
#    Updated: 2025/05/29 00:43:32 by marvin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Source path directories
printf = ft_printf/
gnl = get_next_line/

# Object path directories
OBJ_PATH = obj/
OBJ_PATH2 = $(printf)obj/
OBJ_PATH3 = $(gnl)obj/
# all of the above
DIRS = $(OBJ_PATH) $(OBJ_PATH2) $(OBJ_PATH3)

# libft
SRC = ./ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c \
./ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c \
./ft_strchr.c ft_strdup.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strncmp.c ft_strnstr.c ft_strrchr.c \
./ft_tolower.c ft_toupper.c \
./ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c \
./ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
./ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c \

# ft_printf
SRC_PRINTF = $(printf)ft_printf.c $(printf)ft_printf_str.c $(printf)ft_printf_unsigned.c $(printf)ft_printf_ptr.c $(printf)ft_printf_hex.c \

# get_next_line (bonus 1/2)
SRC_GNL = $(gnl)get_next_line.c $(gnl)get_next_line_utils.c \

# Object files
OBJ = $(SRC:%.c=$(OBJ_PATH)%.o)
OBJ_PRINTF = $(SRC_PRINTF:$(printf)%.c=$(OBJ_PATH2)%.o)
OBJ_GNL = $(SRC_GNL:$(gnl)%.c=$(OBJ_PATH3)%.o)

# Compiler n flags
CC		= cc
CFLAGS += -Wall -Werror -Wextra -I. -I$(printf) -I$(gnl)

# Build name
NAME = libft.a

# Build magicc
all:$(NAME)

$(NAME)	: $(DIRS) $(OBJ) $(OBJ_PRINTF) $(OBJ_GNL)
	@ar -rcs $(NAME) $(OBJ) $(OBJ_PRINTF) $(OBJ_GNL)

$(OBJ_PATH)%.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH2)%.o : $(printf)%.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH3)%.o : $(gnl)%.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)

$(OBJ_PATH2):
	@mkdir -p $(OBJ_PATH2)

$(OBJ_PATH3):
	@mkdir -p $(OBJ_PATH3)

clean:
	@rm -rf $(OBJ_PATH) $(OBJ_PATH2) $(OBJ_PATH3)

fclean: clean
	@rm -f $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re