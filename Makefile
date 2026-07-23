NAME        = codexion

SRC         = main.c \
              parsing.c \
              threads.c \
              coders.c \
              dongles.c \
              monitor.c \
              scheduler.c \
              clean.c \
              utils.c

BUILD_DIR   = obj/
OBJ         = $(addprefix $(BUILD_DIR), $(SRC:.c=.o))
DEPS        = $(OBJ:.o=.d)

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -pthread -MMD -MP

all: $(NAME)

$(NAME): $(OBJ) codexion.h
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(BUILD_DIR)%.o: %.c codexion.h
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re