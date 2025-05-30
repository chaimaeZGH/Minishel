CC = cc
CFLAGS = -Wall -Wextra -Werror
READLINE_PATH = $(shell brew --prefix readline)
LDFLAGS = -L$(READLINE_PATH)/lib -lreadline -lncurses
CPPFLAGS = -I$(READLINE_PATH)/include

SRCS = main.c merj.c
OBJS = $(SRCS:.c=.o)

# SRCS_B = 
# OBJS_B = $(SRCS_B:.c=.o)

HEADER = minishell.h
NAME = minishell

# NAME_B = 
# HEADER_B = 

all: $(NAME)

# bonus: $(NAME_B)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

# $(NAME_B): $(OBJS_B)
# 	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS_B) -o $(NAME_B)

# %_bonus.o: %_bonus.c $(HEADER_B)
# 	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
# 	# rm -f $(OBJS_B)

fclean: clean
	rm -f $(NAME)
# 	# rm -f $(NAME_B)

re: fclean all

.PHONY: all clean fclean re