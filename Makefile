NAME        = exec
CC          = cc
FLAGS       = -Wall -Wextra -Werror

SRCS   = 	builtins/cd.c builtins/echo.c builtins/env.c builtins/exit.c builtins/export.c builtins/pwd.c \
			libft/ft_atoi.c libft/ft_strncmp.c libft/ft_strlen.c libft/ft_strdup.c libft/ft_putchar_fd.c libft/ft_putendl_fd.c \
			helper/env.c helper/error.c libft/ft_putstr_fd.c libft/ft_strchr.c main.c  libft/ft_isalnum.c libft/ft_substr.c libft/ft_isalpha.c \
			libft/ft_isdigit.c builtins/unset.c helper/convert.c libft/ft_strjoin.c libft/ft_split.c helper/free.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re