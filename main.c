/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sboulain <sboulain@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 13:29:45 by omathot           #+#    #+#             */
/*   Updated: 2023/07/12 12:14:12 by sboulain         ###   ########.fr       */
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

printf, malloc, free, write, access, open, read,
close, fork, wait, waitpid, wait3, wait4, signal,
sigaction, sigemptyset, sigaddset, kill, exit,
, stat, lstat, fstat, unlink, execve,
dup, dup2, pipe, opendir, readdir, closedir,
strerror, perror, isatty, ttyname, ttyslot, ioctl,
, tcsetattr, tcgetattr, tgetent, tgetflag,
tgetnum, tgetstr, tgoto, tputs
*/

/*
*/

char *executer(char *str, bool no_pipe);
t_data_table	*parsse_things(char *str);

int	main(void)
{
	char *str;
	t_data_table *table_pars;
	int	i;

	while (1)
	{
		// intial prompt print
		// read user input
		str = read_user_input();
		// parse user input
		table_pars = parsse_things(str);
		i = 0;
		// c'est pas idea mais c'est un depart
		// while (table_pars->list_of_commands[i] != NULL)
		// {
		// 	// break up components of user input (specifically take care with quotes)
		// 	// verify if correct user input (if not error or ask for more)
		// 	// expand PATH

		// 	//il faudrai dup fd[0] pour recuper ce qui est imprimer et le resorting en arg
		// 	// execute
		// 	executer(table_pars->list_of_commands[i], table_pars->number_pip == 0);
		// 	i++;
		// }
		
		// optiona: wait for return value.
	}
	return (0);
}

char	*read_user_input(void)
{
	char	*str;
	
	str = readline("minishell_OS_1.0$ 😀 ");
	add_history(str);
	
	return (str);
}
