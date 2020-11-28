# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: sverschu </var/mail/sverschu>                +#+                      #
#                                                    +#+                       #
#    Created: 2019/10/30 16:10:10 by sverschu      #+#    #+#                  #
#    Updated: 2020/11/28 16:52:52 by sverschu      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

# C sourcefiles
SRC = criterion_libft.c $(LIBFT_FOLDER)/libft.a

# compiler and linker
CC = clang
CC_FLAGS = -Werror -Wextra -Wall -lcriterion

# commands
all: libft $(NAME)

$(NAME): $(SRC)
	@$(CC) $(CC_FLAGS) $(EXT_FLAGS) -I$(LIBFT_INC) -I$(LIBFT_FOLDER)/inc -I$(LIBT_FOLDER)/Inc -I$(LIBFT_FOLDER)/Includes -I$(LIBFT_FOLDER)/includes $(SRC) -o $(NAME)

clean:
	@$(RM) $(OBJ)

fclean: clean
	@$(RM) $(NAME)

libft: 
	@$(MAKE) -C $(LIBFT_FOLDER) re

love: all
	@echo "Functions like split and trim are open to interpretation; your idea of these functions might be different than mine."
	@./$(NAME)
re: fclean all
