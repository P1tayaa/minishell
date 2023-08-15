/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 17:01:53 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/08/14 17:04:53 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strtok(char *str, const char *delim)
{
	static char	*last_token;

	last_token = NULL;
	if (str == NULL)
        str = last_token;  // Use the last string if NULL is passed
	int idx = 0;
	while (str[idx] != '\0')
	{
		if (strchr(delim, str[idx]))
			break;
		idx++;
	}
	if (str[idx] == '\0')
	{
        last_token = NULL;
        return (str);
    }
	str[idx] = '\0';
	last_token = &str[idx + 1];
	return (str);
}

void	concat_path(char *buffer, const char *dir, const char *cmd)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (dir[i] != '\0')
	{
		buffer[i] = dir[i];
		i++;
	}
	buffer[i] = '/';
	i++;
	while (cmd[j] != '\0')
	{
		buffer[i] = cmd[j];
		i++;
		j++;
	}
	buffer[i] = '\0';
}
