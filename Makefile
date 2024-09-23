NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror
LIB = -lpthread
SRC = \
		./src/utils.c \
		./src/main.c \
		./src/init.c \

OBJ = $(SRC:.c=.o)

.c.o:
	@$(CC) $(CFLAGS) $< -o $@

$(NAME): $(LIBFT) $(SRC)
		$(CC) $(SRC) $(CFLAGS) $(LIB) -o $(NAME)

all: $(NAME)

clean:
		@rm -rf $(OBJ)

fclean: clean
		@rm -rf $(NAME)

re:	fclean all

valgrind_1death: all
		valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME) 4 310 200 100

valgrind_nodeath: all
		valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME) 5 800 200 200

valgrind_1death_dr: all
		valgrind --tool=helgrind ./$(NAME) 4 310 200 100

valgrind_nodeath_dr: all
		valgrind --tool=helgrind ./$(NAME) 5 800 200 200
	
valgrind_all_ate: all
		valgrind --tool=helgrind ./$(NAME) 5 800 200 200 7

valgrind_solo_philo: all
		valgrind --tool=helgrind ./$(NAME) 1 800 200 200

.PHONY: all clean fclean re