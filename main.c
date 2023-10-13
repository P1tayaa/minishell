/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 13:29:45 by omathot           #+#    #+#             */
/*   Updated: 2023/10/13 13:39:48 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		main(void); 
char	*read_user_input(bool quotes_test);
char	is_prompt_empty(char *str);
char *executer(t_lexer **lexer, t_pipedata *data);
t_lexer	**main_parser(char *str);
void	check_quotes(char **str_og, t_post_quotes ***content);
t_lexer	**parser_with_quotes(t_post_quotes **content);

// void	builtins(t_lexer **lexer)
// {
// 	int	lex_count;

// 	lex_count = 0;
// 	while (lexer[lex_count])
// 	{
// 		puts ("I am trying to launch executer function");
// 		if (ft_memcmp(lexer[lex_count]->cmd, "exit", ft_strlen(lexer[lex_count]->cmd)) == 0)
// 		{
// 			if (!(ft_memcmp(lexer[lex_count]->tokenid, "|", ft_strlen(lexer[lex_count]->tokenid))))
// 				executer(lexer, &data);
// 			else
// 				executer(lexer[lex_count]->cmd, false);
// 		}
// 		else if (ft_memcmp(lexer[lex_count]->cmd, "clear", ft_strlen(lexer[lex_count]->cmd)) == 0)
// 			executer(lexer[lex_count]->cmd, true);
// 		else if (ft_memcmp(lexer[lex_count]->cmd, "pwd", ft_strlen(lexer[lex_count]->cmd)) == 0)
// 			executer(lexer[lex_count]->cmd, true);
// 		else if (ft_memcmp(lexer[lex_count]->cmd, "cd", ft_strlen(lexer[lex_count]->cmd)) == 0)
// 			executer(lexer[lex_count]->cmd, true);
// 		else if (ft_memcmp(lexer[lex_count]->cmd, "echo", ft_strlen(lexer[lex_count]->cmd)) == 0)
// 			executer(lexer[lex_count]->cmd, true);
// 		lex_count++;
// 	}
// }

int    main(void)
{
    char			*str;
    t_lexer			**lexer;
    bool			quotes_test;
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
		if (is_prompt_empty(str) == 0)
			continue ;
		// if (str == NULL)
		// 	continue ;
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
				// printf("arg was before: %s\n", lexer[i]->args);
				// lexer[i]->args = replace_doll_question_to_number_with_free(lexer[i]->args, 69);
				printf("\nlexer[%d]\n cmd: (%s)\n", i, lexer[i]->cmd);
				printf("args: (%s)\n", lexer[i]->args);
				printf("tokenid: (%s)\n", lexer[i]->tokenid);
				printf("file: (%s)\n", lexer[i]->file);
				printf("flags: (%s)\n", lexer[i]->flags);
				i++;
			}
			piping(lexer);
			// pause();
			// pause();
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
