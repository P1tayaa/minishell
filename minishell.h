/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 13:30:26 by omathot           #+#    #+#             */
/*   Updated: 2023/10/12 20:16:33 by oscarmathot      ###   ########.fr       */
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
#include <sys/wait.h>

typedef struct s_post_quotes
{
	char	*content;
	bool	is_quotes;
	bool	have_to_expand;
}	t_post_quotes;

typedef struct s_lexer
{
	char	tokenid[3];
	char	*content;
	char	*cmd;
	char	*flags;
	char	*args;
	char	*file;
	int		possition;
	bool	execd;
}	t_lexer;

typedef	struct s_list_of_quotes
{
	int	*single_quotes;
	int	*double_quotes;
}	t_list_of_quotes;

typedef struct s_pipedata
{
	int		input_fd;
	int		lex_count;
	int		fd[2];
	t_lexer	**lexer;
}	t_pipedata;

void	manage_signals(void);
void	exec(t_lexer *lexer);
void	piping(t_lexer **lexer);
char	*ft_strtok(char *str, const char *delim);
void	concat_path(char *buffer, const char *dir, const char *cmd);
char *str_dup_until_index(char *str, int index);
char *executer(t_lexer **lexer, t_pipedata *data);
int	is_built_in(t_lexer **lexer, t_pipedata *data);

#endif