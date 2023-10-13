/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 13:56:36 by sboulain          #+#    #+#             */
/*   Updated: 2023/10/13 14:06:06 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo_handle(char *str, bool has_n);
void	rtrim(char *str);
bool	ft_isspace(unsigned char c);
void	check_quotes(char *str);

char *executer(t_lexer **lexer, t_pipedata *data)
{
	extern char	**environ;
	char		*return_val;
	char		str2[4096];
	int			i;

	i = 0;
	return_val = NULL;
	if (ft_memcmp(lexer[(*data).lex_count]->cmd, "clear", 5) == 0)
	{
    	write(1, "\033[H\033[2J", 7);
		clear_history();
	}
	else if (ft_memcmp(lexer[(*data).lex_count]->cmd, "exit", 4) == 0)
	{
		printf("exit\n");
		exit(EXIT_SUCCESS);
	}
	else if (ft_memcmp(lexer[(*data).lex_count]->cmd, "pwd", 3) == 0)
	{
		getcwd(str2, 4096);
		printf("%s\n", str2);
		return (return_val);
	}
	else if (ft_memcmp(lexer[(*data).lex_count]->cmd, "echo", 4) == 0)
	{
		if (lexer[(*data).lex_count]->flags != NULL)
		{
			if (ft_memcmp(lexer[(*data).lex_count]->flags, "-n", 2) == 0)
			{
				echo_handle(lexer[(*data).lex_count]->args, true);
				return (return_val);
			}
		}
		echo_handle(lexer[(*data).lex_count]->args, false);
	}
	else if (ft_memcmp(lexer[(*data).lex_count]->cmd, "cd", 2) == 0)
	{
		if (chdir(lexer[(*data).lex_count]->args) != 0)
			perror("chdir() error");
	}
	else if (ft_memcmp(lexer[(*data).lex_count]->cmd, "env", 3) == 0)
	{
		while (environ[i])
			printf("%s\n", environ[i++]);
	}
	else if (ft_memcmp(lexer[(*data).lex_count]->cmd, "^D", 2) == 0)
    	exit (EXIT_SUCCESS);
	// else
	// {
	// 	write(1, "proble la\n", 10);
	// 	check_quotes(str);
	// 	// printf("minishell: %s: command not found\n", str);
	// }
	return (return_val);
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
	while (len > 0 && ft_isspace(str[len - 1]))
        str[--len] = '\0';
}

bool	ft_isspace(unsigned char c)
{
	if (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
		|| c == ' ')
		return (true);
	return (false);
}
