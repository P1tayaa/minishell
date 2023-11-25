/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_8.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 23:02:18 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 23:02:30 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	return_biggest_int(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

char	*here_doc_starter(char *wordlocking_for)
{
	char	*str_return;
	char	*read_line_str;

	rl_catch_signals = 0;
	str_return = NULL;
	read_line_str = readline(" > ");
	while (read_line_str != NULL && ft_strncmp(wordlocking_for,
			read_line_str, return_biggest_int(ft_strlen(wordlocking_for),
				ft_strlen(read_line_str))) != 0 && g_exit_status == 0)
	{
		str_return = sjoin_fr(str_return, ft_strjoin("\n", read_line_str));
		read_line_str = readline(" > ");
	}
	if (g_exit_status != 0)
	{
		g_exit_status = 0;
		exit(130);
	}
	str_return = sjoin_fr(str_return, ft_strdup("\n"));
	return (str_return);
}

bool	is_str_export(char *str)
{
	if (ft_strlen(str) == 6)
		if (str[0] == 'e' && str[1] == 'x' && str[2] == 'p'
			&& str[3] == 'o' && str[4] == 'r' && str[5] == 't'
			&& str[6] == '\0')
			return (true);
	return (false);
}

bool	is_str_unset(char *str)
{
	if (!str)
		if (ft_strlen(str) != 5)
			return (false);
	if (str[0] == 'u' && str[1] == 'n' && str[2] == 's'
		&& str[3] == 'e' && str[4] == 't' && str[5] == '\0')
		return (true);
	return (false);
}

char	*ft_strdup_until_space(char *str)
{
	int	i;

	i = 0;
	while (!ft_isspace(str[i]) && str[i] != '\0')
		i++;
	return (str_dup_until_index(str, i));
}
