# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: sverschu </var/mail/sverschu>                +#+                      #
#                                                    +#+                       #
#    Created: 2019/10/30 16:10:10 by sverschu      #+#    #+#                  #
#    Updated: 2019/11/13 13:01:58 by sverschu      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

# ONLY CHANGE "LIBFT_FOLDER"
LIBFT_FOLDER = ../libft

# ONLY CHANGE IF THE GODS HAVE DECIDED TO USE PROPER FOLDERS
LIBFT_INC = $(LIBFT_FOLDER)

NAME = run-test

# C sourcefiles
SRC = criterion_libft.c $(LIBFT_FOLDER)/libft.a

# compiler and linker
CC_FLAGS = -Werror -Wextra -Wall -lcriterion

# commands
all: libft $(NAME)

$(NAME): $(SRC)
	@$(CC) $(CC_FLAGS) -I$(LIBFT_INC) $(SRC) -o $(NAME)

clean:
	@$(RM) $(OBJ) $(BONUS_OBJ)

fclean: clean
	@$(RM) $(NAME)

libft: 
	@$(MAKE) -C $(LIBFT_FOLDER) re

f: all
	@echo "Segfault errors means your functions are overprotected (but Moulinette will love you anyway)"
	@./$(NAME)
re: fclean all
