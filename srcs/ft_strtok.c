/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 23:03:39 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 23:10:15 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strtok(char *str, const char *delim)
{
	int			i;
	int			index_unter_delim;
	static int	curser;
	static char	*str_reminder;
	char		*return_char;

	if (str != NULL)
	{
		curser = 0;
		str_reminder = str;
	}
	if (str_reminder == NULL)
		return (NULL);
	index_unter_delim = ft_char_find(&str_reminder[curser], delim);
	if (index_unter_delim == -1)
		if (str_reminder[curser] == '\0'
			&& ft_strtok_line_help(&str_reminder, &index_unter_delim, &curser))
			return (NULL);
	return_char = malloc(sizeof(char) * (index_unter_delim + 1));
	i = -1;
	while (++i < index_unter_delim)
		return_char[i] = str_reminder[curser + i];
	curser = curser + 1;
	return_char[i] = '\0';
	return (return_char);
}
