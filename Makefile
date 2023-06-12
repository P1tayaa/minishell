# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: omathot <omathot@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/09 01:41:47 by oscarmathot       #+#    #+#              #
#    Updated: 2023/06/12 11:54:52 by omathot          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#---------------------------------
#VARIABLES

NAME 	:= minishell
SRC 	:= main.c quotes.c
OBJ 	:= $(SRC:.c=.o)
LIBFT_A	:= lib/libft/libft.a
CMP		:= gcc
FLAGS 	:= -Werror -Wall -Wextra -g -I lib

#---------------------------------
#FORMATTING AND FUN

clear_line = \033[K
move_up = \033[A
define prettycomp
	@printf "$(1)$(clear_line)\n"
	@$(1)
	@printf "$(move_up)"
endef

RED		:= \033[31m
GREEN 	:= \033[32m
YELLOW	:= \033[33m
BLUE	:= \033[38;5;87m
MAGENTA := \033[35m
CYAN	:= \033[36m
RESET	:= \033[0m

#---------------------------------
#RULES

all	: $(NAME)
		@echo "$(GREEN)Project built successfully !$(RESET)"
		@echo "$(BLUE)Usage: so_long <path to map>$(RESET)"

$(NAME) : $(OBJ) $(LIBFT_A) minishell.h
		@echo "$(CYAN)Creating the executable...$(RESET)"
		@$(CC) $(FLAGS) -lreadline $(OBJ) $(LIBFT_A) -o $(NAME)

%.o : %.c minishell.h
		@echo "$(CYAN)Compiling...$(RESET) $<"
		$(call prettycomp, $(CMP) -c $(FLAGS) -o $@ $<)

$(LIBFT_A) : 
		@echo "$(BLUE)Building libft library...$(RESET)\n"
		$(call prettycomp, @make -C lib/libft)

clean :
		@rm -f $(OBJ) ./lib/libft/libft.a
		@echo "$(GREEN)Cleaned up the artifacts !$(RESET)"

fclean :
		@rm -f $(NAME) $(OBJ)
		@echo "$(MAGENTA)Cleaned up executable !$(RESET)"

hardclean : 
		@make fclean
		cd ./lib/libft && make clean
		cd ./lib/libft && make fclean
		@echo "$(MAGENTA)Cleaned up all built files!$(RESET)"

re : fclean all
	
.PHONY : clean fclean re hardclean