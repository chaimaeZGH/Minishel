CC = cc
CFLAGS = -Wall -Wextra -Werror
READLINE_PATH = $(shell brew --prefix readline)
LDFLAGS = -L$(READLINE_PATH)/lib -lreadline -lncurses
CPPFLAGS = -I$(READLINE_PATH)/include

SRCS = main.c nodes.c creat_tree.c print_tree.c builtins/cd.c builtins/echo.c builtins/env.c builtins/exit.c builtins/export.c builtins/pwd.c \
			libft/ft_atoi.c libft/ft_strncmp.c libft/ft_strlen.c libft/ft_strdup.c libft/ft_putchar_fd.c libft/ft_putendl_fd.c \
			helper/env.c helper/error.c libft/ft_putstr_fd.c libft/ft_strchr.c   libft/ft_isalnum.c libft/ft_substr.c libft/ft_isalpha.c \
			libft/ft_isdigit.c builtins/unset.c helper/convert.c libft/ft_strjoin.c libft/ft_split.c helper/free.c execution/solo_cmd.c \
			execution/ex_builtins.c execution/exec.c execution/execute_pipe.c execution/redirections.c

OBJS = $(SRCS:.c=.o)

# SRCS_B = 
# OBJS_B = $(SRCS_B:.c=.o)

HEADER = minishell.h
NAME = minishell

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

# NAME_B = 
# HEADER_B = 

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

# bonus: $(NAME_B)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)

# $(NAME_B): $(OBJS_B)
# 	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS_B) -o $(NAME_B)

# %_bonus.o: %_bonus.c $(HEADER_B)
# 	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c $(HEADER) $(LIBFT_DIR)/libft.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean
# 	# rm -f $(OBJS_B)

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean
# 	# rm -f $(NAME_B)

re: fclean all

.PHONY: all clean fclean re