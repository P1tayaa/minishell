/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 13:56:36 by sboulain          #+#    #+#             */
/*   Updated: 2023/10/31 20:06:56 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo_handle(char *str, bool has_n);
void	rtrim(char *str);
bool	ft_isspace(unsigned char c);
void	check_quotes(char *str);
char	***get_env(void);
void	set_env(char *name, char *value, char ***environment);
int		unset_env(char *name, char ***environment);
void	print_env(char ***environment);
void	print_export(char ***environment);
void	ascii_sort(char **environment);
void	free_double_array(char **list_of_tokenid);
char	***get_env(void);

int executer(t_lexer **lexer, t_pipedata *data)
{
	int			return_val;
	char		str2[4096];
	// int			i;

	// i = 0;
	return_val = 0;
	if (lexer[(*data).lex_count]->cmd == NULL)
		return(0);
	if (ft_memcmp(lexer[(*data).lex_count]->cmd, "clear", 5) == 0)
	{
    	write(1, "\033[H\033[2J", 7);
		clear_history();
	}
	else if (ft_memcmp(lexer[(*data).lex_count]->cmd, "exit", 4) == 0)
	{
		if (lexer[(*data).lex_count]->args != NULL)
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(lexer[(*data).lex_count]->args, STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			free_double_array(*(get_env)());
			exit(2);
		}
		exit(EXIT_SUCCESS);
	}
	else if (ft_memcmp(lexer[(*data).lex_count]->cmd, "pwd", 3) == 0)
	{
		if (getcwd(str2, 4096) == NULL)
			write(2, "invalid directory\n", 18);
		else
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
	
	//----------------------------- !! environment stuff !! ----------------------------------------

	else if (ft_memcmp(lexer[(*data).lex_count]->cmd, "env", 3) == 0)
		print_env(get_env());
	// else if (ft_memcmp(lexer[(*data).lex_count]->cmd, "unset", 5) == 0)
	// 	unset_env(ft_strdup("x"), get_env());
	// // else if (ft_memcmp(lexer[(*data).lex_count]->cmd, "export x", 8) == 0)
	// // 	set_env(ft_strdup("x"), ft_strdup("10"), get_env());
	// else if (ft_memcmp(lexer[(*data).lex_count]->cmd, "export", 6) == 0)
	// 	ascii_sort((*get_env()));
	// else if (ft_memcmp(lexer[(*data).lex_count]->cmd, "poop", 4) == 0)
	// {
	// 	set_env(ft_strdup("x"), ft_strdup("\0"), get_env());
	// }
	
	//--------------------------------------------------------------------------------------------
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
