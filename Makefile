# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/09 01:41:47 by oscarmathot       #+#    #+#              #
#    Updated: 2023/11/25 23:19:13 by oscarmathot      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#---------------------------------
#VARIABLES

NAME 	:= minishell
SRC 	:= main.c quotes.c executer.c parser.c signals.c pipe.c free_help.c utils.c environment.c environment_export.c \
			manage_environment.c parser_flags.c parser_nn.c parser_quotes.c parser_remove.c parser_starter.c parser_token.c \
			parser_utils.c utils_2.c quotes_acronyms.c quotes_check_quotes.c quotes_doll.c quotes_index.c quotes_lookup.c \
			quotes_prep.c quotes_spit.c quotes_utils.c main_helper.c main_helper2.c utils_3.c utils_4.c utils_5.c utils_6.c \
			utils_7.c utils_8.c utils_9.c ft_strtok.c pipe_export.c pipe_helper.c pipe_helper2.c pipe_process.c pipe_process2.c \
			pipe_reads_writes.c pipe_setup.c
SUBDIR	:= srcs/
SRCS	:= $(addprefix $(SUBDIR),$(SRC))
OBJ 	:= $(SRCS:.c=.o)
LIBFT_A	:= lib/libft/libft.a
CMP		:= gcc
# FLAGS 	:= -Werror -Wall -Wextra -g -I lib
FLAGS 	:= -Werror -Wall -Wextra -fsanitize=address -g -I lib # 

ifeq ($(wildcard $(/opt/homebrew/opt/readline/include)),)
	FLAGS = -Werror -Wall -Wextra -fsanitize=address -g -I lib -I/opt/homebrew/opt/readline/include
endif

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
		@echo "a\\n"
		@echo "$(GREEN)Project built successfully !$(RESET)"

$(NAME) : $(OBJ) $(LIBFT_A) minishell.h
		@echo "$(CYAN)Creating the executable...$(RESET)"
		@$(CC) $(FLAGS) $(OBJ) $(LIBFT_A) -L/opt/homebrew/opt/readline/lib -lreadline -o $(NAME)

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
