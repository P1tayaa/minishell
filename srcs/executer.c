/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 13:56:36 by sboulain          #+#    #+#             */
/*   Updated: 2023/11/28 21:24:25 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	is_there_space(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
			return (1);
		i++;
	}
	return (0);
}

int	executer_p3(t_lexer **lexer, t_pipedata *data)
{
	if (ft_memcmp(lexer[(*data).lex_count]->cmd, "cd", 2) == 0)
	{
		if (chdir(lexer[(*data).lex_count]->args) != 0)
		{
			ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
			ft_putstr_fd(lexer[(*data).lex_count]->args, STDERR_FILENO);
			perror(" ");
			return (1);
		}
	}
	else if (ft_memcmp(lexer[(*data).lex_count]->cmd, "env", 3) == 0)
		print_env(get_env());
	else if (ft_memcmp(lexer[(*data).lex_count]->cmd, "^D", 2) == 0)
		exit (EXIT_SUCCESS);
	return (0);
}

int	executer_p2(t_lexer **lexer, t_pipedata *data)
{
	char		str2[4096];

	if (ft_memcmp(lexer[(*data).lex_count]->cmd, "pwd", 3) == 0)
	{
		if (getcwd(str2, 4096) == NULL)
		{
			write(2, "invalid directory\n", 18);
			g_exit_status = 126;
			return (126);
		}
		else
			printf("%s\n", str2);
	}
	else if (ft_memcmp(lexer[(*data).lex_count]->cmd, "echo", 4) == 0)
	{
		if (lexer[(*data).lex_count]->flags != NULL)
		{
			if (ft_memcmp(lexer[(*data).lex_count]->flags, "-n", 2) == 0)
				return (echo_handle(lexer[(*data).lex_count]->args, true), 0);
		}
		echo_handle(lexer[(*data).lex_count]->args, false);
	}
	else
		executer_p3(lexer, data);
	return (0);
}

int	is_there_letter(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((str[i] >= 65 && str[i] <= 90) || (str[i] >=97 && str[i] <= 122))
			return (1);
		i++;
	}
	return (0);
}

int	executer(t_lexer **lexer, t_pipedata *data)
{
	int			return_val;

	return_val = 0;
	if (lexer[(*data).lex_count]->cmd == NULL)
		return (0);
	if (ft_memcmp(lexer[(*data).lex_count]->cmd, "clear", 5) == 0)
	{
		write(1, "\033[H\033[2J", 7);
		clear_history();
	}
	else if (ft_memcmp(lexer[(*data).lex_count]->cmd, "exit", 4) == 0)
	{
		if (lexer[(*data).lex_count]->args != NULL)
		{
			if (is_there_space(lexer[(*data).lex_count]->args))
			{
				write(2, "exit\nminishell: exit: too many arguments\n", 41);
				return (1);
			}
			else if (is_there_letter(lexer[(*data).lex_count]->args))
			{
				ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
				ft_putstr_fd(lexer[(*data).lex_count]->args, STDERR_FILENO);
				ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
				free_double_array(*(get_env)());
				exit(2);
			}
			exit(ft_atoi(lexer[(*data).lex_count]->args));
		}
		exit (EXIT_SUCCESS);
	}
	else
		return_val = executer_p2(lexer, data);
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
		return ;
	while (len > 0 && ft_isspace(str[len - 1]))
		str[--len] = '\0';
}
