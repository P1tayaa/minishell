/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 13:29:45 by omathot           #+#    #+#             */
/*   Updated: 2023/10/04 17:22:57 by oscarmathot      ###   ########.fr       */
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
t_lexer	**main_parser(char *str);
void	check_quotes(char **str_og, t_post_quotes ***content);
t_lexer	**parser_with_quotes(t_post_quotes **content);

int	main(void)
{
	char	*str;
	t_lexer	**lexer;
	bool	quotes_test;
	t_post_quotes	**content;
	// int	i;

	manage_signals();
	quotes_test = false;
	while (1)
	{
		// intial prompt print
		// read user input
		str = read_user_input();
		if (!(is_prompt_empty(str)))
			continue ;
		if (quotes_test)
		{
			check_quotes(&str, &content);
			puts(str);
			add_history(str);
		}
		// parse user input
		// if (!quotes_test)
		// {
			// pause();
			// puts("");
			// puts(str);

			if (content == NULL)
				lexer = main_parser(str);
			else
				lexer = parser_with_quotes(content);
			// i = 0;
			// c'est pas idea mais c'est un depart
			int i;
			i = 0;
			while (lexer[i] != NULL)
			{
				printf("\nlexer[%d]\n cmd: (%s)\n", i, lexer[i]->cmd);
				printf("args: (%s)\n", lexer[i]->args);
				printf("tokenid: (%s)\n", lexer[i]->tokenid);
				printf("file: (%s)\n", lexer[i]->file);
				printf("flags: (%s)\n", lexer[i]->flags);
				i++;
			}
			
			if (lexer[1] == NULL)
				exec(lexer[0]);
			else
				piping(lexer);
			// optiona: wait for return value.
		// }
		
	}
	return (0);
}

char	is_prompt_empty(char *str)
{
	int		i;
	char	is_valid;

	i = 0;
	is_valid = 0;
	while (str[i])
		i++;
	if (i > 0)
		is_valid = 1;
	return (is_valid);
}

char	is_prompt_empty(char *str)
{
	int		i;
	char	is_valid;

	i = 0;
	is_valid = 0;
	while (str[i])
		i++;
	if (i > 0)
		is_valid = 1;
	return (is_valid);
}

char	*read_user_input(bool quotes_test)
{
	char	*str;
	
	str = readline("😎 minishell_OS_1.0$ ");
	if (!quotes_test)
		add_history(str);
	// if (EOF)
	// {
	// 	write(1, "exit\n", 5);
	// 	exit(EXIT_SUCCESS);
	// }
	return (str);
}
