/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omathot <omathot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 13:29:45 by omathot           #+#    #+#             */
/*   Updated: 2023/06/12 10:39:44 by omathot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void); 
char	**read_user_input(void);
void	echo_handle(char *str, bool has_n);
void	rtrim(char *str);
int		ft_isspace(int c);


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


printf, malloc, free, write, access, open, read,
close, fork, wait, waitpid, wait3, wait4, signal,
sigaction, sigemptyset, sigaddset, kill, exit,
, stat, lstat, fstat, unlink, execve,
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
	char	*str;
	char	str2[4096];
	
	str = readline("minishell_OS_1.0$ ");
	add_history(str);
	if (ft_memcmp(str, "clear", ft_strlen(str)) == 0)
	{
    	write(1, "\033[H\033[2J", 7);
		clear_history();
		
	}
	if (ft_memcmp(str, "exit", 4) == 0)
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
	if (ft_memcmp(str, "cd ", 3) == 0)
	{
		rtrim(&str[3]);
		if (chdir(&str[3]) != 0)
			perror("chdir() error");
	}
	return (NULL);
}

void	echo_handle(char *str, bool has_n)
{
	write(1, str, ft_strlen(str));
	if (!has_n)
		write(1, "\n", 1);	
}

void	rtrim(char *str)
{
	int	len;

	len = ft_strlen(str);
	if (str == NULL)
		return;
	while (len > 0 && ft_isspace((unsigned char) str[len - 1]))
        str[--len] = '\0';
}

int		ft_isspace(int c)
{
	c = (unsigned char)c;
	if (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
		|| c == ' ')
		return (1);
	return (0);
}
