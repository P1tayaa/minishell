/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 13:29:45 by omathot           #+#    #+#             */
/*   Updated: 2023/08/15 13:08:00 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void); 
char	*read_user_input(void);



/*
void add_history (const char *string)
Place string at the end of the history list. The associated data field (if any) is set to NULL.

void clear_history (void)
Clear the history list by deleting all the entries.

char *
    readline (const char *prompt);

rl_on_new_line,
rl_nreplace_lie, rl_redisplay,

getcwd
chdir
getenv

dup, dup2, pipe    PIPES
duplicate stdout environment, have shell write on there, return next stdout environment. 

access !!!

printf, malloc, free, write, , open, read,
close, fork, wait, waitpid, wait3, wait4, signal,
sigaction, sigemptyset, sigaddset, kill, exit,
, stat, lstat, fstat, unlink, execve,
, opendir, readdir, closedir,
strerror, perror, isatty, ttyname, ttyslot, ioctl,
, tcsetattr, tcgetattr, tgetent, tgetflag,
tgetnum, tgetstr, tgoto, tputs
*/

/*
feof(3), ferror(3), fgetln(3), fgetws(3), getline(3)
*/

char *executer(char *str, bool no_pipe);
t_lexer	**parsse_things(char *str);

int	main(void)
{
	char	*str;
	t_lexer	**lexer;
	// int	i;

	manage_signals();
	while (1)
	{
		// intial prompt print
		// read user input
		str = read_user_input();
		// parse user input
		lexer = parsse_things(str);
		// i = 0;
		// c'est pas idea mais c'est un depart
		if (lexer[1] == NULL)
			exec(lexer[0]);
		else
			piping(lexer);
		// optiona: wait for return value.
	}
	return (0);
}

char	*read_user_input(void)
{
	char	*str;
	
	str = readline("😎 minishell_OS_1.0$ ");
	add_history(str);
	// if (EOF)
	// {
	// 	write(1, "exit\n", 5);
	// 	exit(EXIT_SUCCESS);
	// }
	return (str);
}
