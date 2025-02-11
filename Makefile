NAME = ircserv

CC = c++
CFLAGS =  -g -MMD -MP -Wall -Wextra -Werror -std=c++98 -Iincludes

SRC_DIR = ./sources/

SRC = $(shell find $(SRC_DIR) -name "*.cpp")

OBJ_DIR = ./OBJ/

OBJ = $(patsubst $(SRC_DIR)%.cpp, $(OBJ_DIR)%.o, $(SRC))

DEP = $(OBJ:%.o=%.d)

INC_DIR = includes/

INC = SRC = $(shell find $(INC_DIR) -name "*.hpp")

all: $(NAME)

-include $(DEP)

clean: 
	rm -rf $(OBJ_DIR)
	@echo "clean done"

fclean: clean
	rm -rf $(NAME)
	@echo "fclean done"

re: fclean all

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@
	@echo "$(NAME) done"

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ 

.PHONY: all clean fclean re
