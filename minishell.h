/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 13:30:26 by omathot           #+#    #+#             */
/*   Updated: 2023/07/20 15:15:57 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <signal.h>
# include "./lib/libft/libft.h"
# include <fcntl.h>

typedef struct s_lexer
{
	char tokenid[3];
	char *content;
	int	possition;
	
}	t_lexer;

typedef struct s_data_table
{
    int     number_pip;
    char    **list_of_commands;
    
}       t_data_table;

void	manage_signals(void);
void	redir(t_lexer *cmd, char **env, int fdin);
int		piping(int argc, char **argv, char **env);
void	free_parser(t_lexer **lexer);


#endif