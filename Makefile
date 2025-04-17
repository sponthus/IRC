## IRC server variables

NAME = ircserv

SRC_DIR = ./sources/

SRC = $(shell find $(SRC_DIR) -name "*.cpp")

OBJ_DIR = ./OBJ/

OBJ = $(patsubst $(SRC_DIR)%.cpp, $(OBJ_DIR)%.o, $(SRC))

## BOT variables

BOT = QuestBot

BOT_DIR = ./bot/

BOT_SRC = $(shell find $(BOT_DIR) -name "*.cpp")

BOT_OBJ_DIR = ./OBJ_BOT/

BOT_OBJ = $(patsubst $(BOT_DIR)%.cpp, $(BOT_OBJ_DIR)%.o, $(BOT_SRC))

## INC & GENERAL

DEP = $(OBJ:%.o=%.d), $(BOT_OBJ:%.o=%.d)

# INC_DIR = includes/

# INC = SRC = $(shell find $(INC_DIR) -name "*.hpp")

CC = c++
CFLAGS =  -g -MMD -MP -Wall -Wextra -Werror -std=c++98 -Iincludes

## Compilation rules

all: $(NAME) $(BOT)

-include $(DEP)

server: $(NAME)

bot: $(BOT)

clean: 
	rm -rf $(OBJ_DIR)
	@echo "clean done"

fclean: clean
	rm -rf $(NAME)
	@echo "fclean done"

re: fclean all

## IRC compilation rules

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@
	@echo "$(NAME) done"

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

## Bot compilation rules

$(BOT): $(BOT_OBJ)
	$(CC) $(CFLAGS) $(BOT_OBJ) -o $@
	@echo "$(BOT) done"

$(BOT_OBJ_DIR)%.o: $(BOT_DIR)%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean fclean re server bot
