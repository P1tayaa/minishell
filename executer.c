/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 13:56:36 by sboulain          #+#    #+#             */
/*   Updated: 2023/10/18 14:56:08 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo_handle(char *str, bool has_n);
void	rtrim(char *str);
bool	ft_isspace(unsigned char c);
void	check_quotes(char *str);

int executer(t_lexer **lexer, t_pipedata *data)
{
	int			return_val;
	char		str2[4096];
	int			i;

	i = 0;
	return_val = 0;
	if (ft_memcmp(lexer[(*data).lex_count]->cmd, "clear", 5) == 0)
	{
    	write(1, "\033[H\033[2J", 7);
		clear_history();
	}
	else if (ft_memcmp(lexer[(*data).lex_count]->cmd, "exit", 4) == 0)
	{
			printf("exit\n");
		if (lexer[(*data).lex_count]->args != NULL)
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(lexer[(*data).lex_count]->args, STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			exit(255);
		}
		exit(EXIT_SUCCESS);
	}
	else if (ft_memcmp(lexer[(*data).lex_count]->cmd, "pwd", 3) == 0)
	{
		getcwd(str2, 4096);
		printf("%s\n", str2);
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
		{
			ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
			ft_putstr_fd(lexer[(*data).lex_count]->args, STDERR_FILENO);
			perror("");
			return(1);
		}
	}
	else if (ft_memcmp(lexer[(*data).lex_count]->cmd, "env", 3) == 0)
	{
		while ((*(*data).environ)[i])
			printf("%s\n", (*(*data).environ)[i++]);
	}
	else if (ft_memcmp(lexer[(*data).lex_count]->cmd, "^D", 2) == 0)
    	exit (EXIT_SUCCESS);
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
