NAME = ircserv

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98

CFL = -MMD -MP

SRC_DIR = sources/

SRC = $(SRC_DIR)main.cpp

OBJ_DIR = ./OBJ/

OBJ = $(SRC:%.cpp=$(OBJ_DIR)%.o)

DEP = $(OBJ:%.o=%.d)

INCS_DIR = includes/

INCS = $(INCS_DIR)

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
	$(CC) $(CFL) $(CFLAGS) $(OBJ) -o $@
	@echo "$(NAME) done"

$(OBJ_DIR)%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFL) $(CFLAGS) -o $@ -c $<

.PHONY: all, clean, fclean, re
