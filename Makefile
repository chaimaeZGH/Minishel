NAME        = exec
CC          = cc
FLAGS       = -Wall -Wextra -Werror



SRCS   = 	builtins/cd.c builtins/echo.c builtins/env.c builtins/exit.c builtins/export.c builtins/pwd.c \
			libft/ft_atoi.c libft/ft_strncmp.c libft/ft_strlen.c libft/ft_strdup.c libft/ft_putchar_fd.c libft/ft_putendl_fd.c

OBJS = $(SRCS:.c=.o)
OBJS_BONUS = $(SRCS_BONUS:.c=.o)




 all: $(NAME)

$(NAME): minishell.h  $(OBJS)
	$(CC) $(FLAGS) $(OBJS)  -o $(NAME)


.c.o:
	$(CC) $(FLAGS) -c $< -o $@
#bonus

clean:
	rm -f $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re