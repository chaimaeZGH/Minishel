CC = cc
CFLAGS = -Wall -Wextra -Werror
READLINE_PATH = /Users/czghoumi/.brew/opt/readline
LDFLAGS = -L$(READLINE_PATH)/lib -lreadline -lncurses
CPPFLAGS = -I$(READLINE_PATH)/include

SRCS = main.c merj.c
OBJS = $(SRCS:.c=.o)

HEADER = minishell.h
NAME = minishell

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
# CC = cc
# CFLAGS = -Wall -Wextra -Werror

# LDFLAGS = -L/opt/homebrew/opt/readline/lib -lreadline -lncurses

# SRCS = main.c merj.c
# OBJS = $(SRCS:.c=.o)

# # SRCS_B = 
# # OBJS_B = $(SRCS_B:.c=.o)

# HEADER = minishell.h
# NAME = minishell

# # NAME_B = 
# # HEADER_B = 

# all: $(NAME)

# bonus: $(NAME_B)

# $(NAME): $(OBJS)
# 	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(NAME)

# # $(NAME_B): $(OBJS_B)
# # 	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS_B) -o $(NAME_B)

# # %_bonus.o: %_bonus.c $(HEADER_B)
# # 	$(CC) $(CFLAGS) -c $< -o $@

# %.o: %.c $(HEADER) 
# 	$(CC) $(CFLAGS) -c $< -o $@

# clean:
# 	rm -f $(OBJS)
# 	# rm -f $(OBJS_B)

# fclean: clean
# 	rm -f $(NAME)
# 	# rm -f $(NAME_B)

# re: fclean all

# .PHONY: all clean fclean re bonus