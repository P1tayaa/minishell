/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 13:30:26 by omathot           #+#    #+#             */
/*   Updated: 2023/08/14 20:20:09 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <string.h>	// REMOOOOOOOOVE BEFORE PUUUUSH! 
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
	char	tokenid[3];
	char	*content;
	char	*cmd;
	char	*flags;
	char	*args;
	int		possition;
}	t_lexer;

void	manage_signals(void);
void	exec(t_lexer *lexer);
void	piping(t_lexer **lexer);
char	*ft_strtok(char *str, const char *delim);
void	concat_path(char *buffer, const char *dir, const char *cmd);

#endif