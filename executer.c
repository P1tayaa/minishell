/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 13:56:36 by sboulain          #+#    #+#             */
/*   Updated: 2023/06/14 14:14:59 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo_handle(char *str, bool has_n);
void	rtrim(char *str);
bool	ft_isspace(unsigned char c);
void	check_quotes(char *str);

char *executer(char *str, bool no_pipe)
{
	extern char	**environ;
	char		*return_val;
	char		str2[4096];
	int			i;
	int			k;

	i = 0;
	k = 0;
	while (ft_isspace(str[i]))
		i++;
	str = &str[i];
	return_val = NULL;
	if (ft_memcmp(str, "clear", 5) == 0)
	{
    	write(1, "\033[H\033[2J", 7);
		clear_history();
	}
	else if (ft_memcmp(str, "exit", 4) == 0)
	{
		if (no_pipe)
		{
			printf("%s\n", str);
			exit(EXIT_SUCCESS);
		}
	}
	else if (ft_memcmp(str, "pwd", 3) == 0)
	{
		getcwd(str2, 4096);
		printf("%s\n", str2);
	}
	else if (ft_memcmp(str, "echo", 4) == 0)
	{
		if (ft_memcmp(&str[4], " -n ", 4) == 0)
			echo_handle(&str[8], true);
		else
			echo_handle(&str[5], false);
	}
	else if (ft_memcmp(str, "cd ", 3) == 0)
	{
		rtrim(&str[3]);
		if (chdir(&str[3]) != 0)
			perror("chdir() error");
	}
	else if (ft_memcmp(str, "env", 3) == 0)
	{
		while (environ[k])
			printf("%s\n", environ[k++]);
	}
	else
	{
		check_quotes(str);
		// printf("minishell: %s: command not found\n", str);
	}
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
