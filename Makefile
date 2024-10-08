NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror
LIB = -lpthread
SRC = \
		./src/main.c \
		./src/free.c \
		./src/init.c \
		./src/monitoring.c \
		./src/parsing.c \
		./src/philo_actions.c \
		./src/routine.c \
		./src/utils.c \

OBJ = $(SRC:.c=.o)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) $(LIB) -o $(NAME)

all: $(NAME)

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re:	fclean all

.PHONY: all clean fclean re