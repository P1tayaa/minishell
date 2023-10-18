/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sboulain <sboulain@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 13:29:45 by omathot           #+#    #+#             */
/*   Updated: 2023/10/12 14:22:10 by sboulain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void); 
char	*read_user_input(bool quotes_test);




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
char *here_doc_starter(char *wordlocking_for);

// ! to remove only here for testing
char *replace_doll_question_to_number_with_free(char *str_og, int number_replace);


int	main(void)
{
	char	*str;
	t_lexer	**lexer;
	bool	quotes_test;
	t_post_quotes	**content;
	int	i;

	manage_signals();
	quotes_test = true;
	while (1)
	{
		// intial prompt print
		// read user input
		content = NULL;
		
		str = ft_strdup(read_user_input(quotes_test));
		if (quotes_test)
		{
			check_quotes(&str, &content);
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
			{
				lexer = parser_with_quotes(content);
				// int i;
				i = 0;
				while (content[i] != NULL)
				{
					printf("int	i == %d\n", i);
					// free(content[i]->)
					free(content[i]->content);
					free(content[i]);
					i++;
				}
				free(content);
			}
			free(str);
			// i = 0;
			// c'est pas idea mais c'est un depart
			int i;
			i = 0;
			while (lexer[i] != NULL)
			{
				printf("arg was before: %s\n", lexer[i]->args);
				lexer[i]->args = replace_doll_question_to_number_with_free(lexer[i]->args, 69);
				printf("\nlexer[%d]\n cmd: (%s)\n", i, lexer[i]->cmd);
				printf("args: (%s)\n", lexer[i]->args);
				printf("tokenid: (%s)\n", lexer[i]->tokenid);
				printf("file: (%s)\n", lexer[i]->file);
				printf("flags: (%s)\n", lexer[i]->flags);
				i++;
				char * str_test;
				str_test = here_doc_starter(lexer[i - 1]->args);
				printf("result : {%s}\n", str_test);
				free(str_test);
			}
			// if (lexer[1] == NULL)
			// 	exec(lexer[0]);
			// else
			// 	piping(lexer);
			
			// optiona: wait for return value.
		// }
		i = 0;
		while (lexer[i] != NULL)
		{
			if (lexer[i]->cmd != NULL)
				free(lexer[i]->cmd);
			if (lexer[i]->args != NULL)
				free(lexer[i]->args);
			if (lexer[i]->file != NULL)
				free(lexer[i]->file);
			if (lexer[i]->flags != NULL)
				free(lexer[i]->flags);
			free(lexer[i]);
			i++;
		}
		free(lexer);
			// break ;
	}
	return (0);
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
