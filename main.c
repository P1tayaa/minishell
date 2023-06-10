/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omathot <omathot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 13:29:45 by omathot           #+#    #+#             */
/*   Updated: 2023/06/10 15:19:08 by omathot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void); 
char	**read_user_input(void);
void	echo_handle(char *str, bool has_n);
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


printf, malloc, free, write, access, open, read,
close, fork, wait, waitpid, wait3, wait4, signal,
sigaction, sigemptyset, sigaddset, kill, exit,
, chdir, stat, lstat, fstat, unlink, execve,
dup, dup2, pipe, opendir, readdir, closedir,
strerror, perror, isatty, ttyname, ttyslot, ioctl,
getenv, tcsetattr, tcgetattr, tgetent, tgetflag,
tgetnum, tgetstr, tgoto, tputs
*/

/*
*/
int	main(void)
{
	while (1)
	{
		// intial prompt print
		read_user_input();
		// read user input
		// parse user input
		// break up components of user input (specifically take care with quotes)
		// verify if correct user input (if not error or ask for more)
		// expand PATH, or whatever (cd, pwd)
		// execute
		// optiona: wait for return value.
	}
	return (0);
}

char	**read_user_input(void)
{
	int		i;
	char	*str;
	char	str2[4096];
	
	i = 0;
	str = readline("minishell_OS_1.0$ ");
	add_history(str);
	if (ft_memcmp(str, "clear", ft_strlen(str)) == 0)
	{
    	write(STDOUT_FILENO, "\033[H\033[2J", 7);
		clear_history();
		
	}
	if (ft_memcmp(str, "exit", ft_strlen(str)) == 0)
	{
		printf("%s\n", str);
		exit(EXIT_SUCCESS);
	}
	if (ft_memcmp(str, "pwd", ft_strlen(str)) == 0)
	{
		getcwd(str2, 4096);
		printf("%s\n", str2);
	}
	if (ft_memcmp(str, "echo", 4) == 0)
	{
		if (ft_memcmp(&str[4], " -n ", 4) == 0)
			echo_handle(&str[8], true);
		else
			echo_handle(&str[5], false);
	}
	return (NULL);
}

void	echo_handle(char *str, bool has_n)
{
	write(1, str, ft_strlen(str));
	if (!has_n)
		write(1, "\n", 1);	
}