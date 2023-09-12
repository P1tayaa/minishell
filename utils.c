/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sboulain <sboulain@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 17:01:53 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/09/09 17:30:27 by sboulain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_char_find(char *str, const char *list_of_char);

char	*ft_strtok(char *str, const char *delim)
{
	int			i;
	int			index_unter_delim;
	static int	curser;
	static char	*str_reminder;
	char	*return_char;

	if (str != NULL)
	{
		curser = 0;
		str_reminder = str;
	}
	if (str_reminder == NULL)
		return (NULL);
	index_unter_delim = ft_char_find(&str_reminder[curser], delim);
	if (index_unter_delim == -1)
	{
		if (str_reminder[curser] == '\0')
			return (NULL);
		index_unter_delim = ft_strlen(&str_reminder[curser]);
	}
	return_char = malloc(sizeof(char) * (index_unter_delim + 1));
	i = 0;
	while (i < index_unter_delim)
	{
		return_char[i] = str_reminder[curser + i];
		i++;
	}
	curser = curser + 1;
	return_char[i] = '\0';
	return (return_char);
}

char *get_string_at_index(char *string, int index)
{
	int	i;

	if (string == NULL)
		return (NULL);
	i = 0;
	while (string[i] != '\0')
	{
		if (i == index)
			return (&string[i]);
		i++;
	}
	return (NULL);
}

int ft_char_find(char *str, const char *list_of_char)
{
	int i;
	int	j;

	if (str == NULL)
		return -1;
	i = 0;
	while (str[i] != '\0')
	{
		j = 0;
		while (list_of_char[j] != '\0')
		{
			if (str[i] == list_of_char[j])
				return (i);
			j++;
		}
		i++;
	}
	return (-1);
}
// {
// 	static char	*last_token;
// 	int			index;

// 	last_token = NULL;
// 	index = 0;
// 	if (str == NULL)
//         str = last_token;					// Use the last string if NULL is passed
// 	while (str[index] != '\0')
// 	{
// 		if (strchr(delim, str[index]))
// 			break;
// 		index++;
// 	}
// 	if (str[index] == '\0')					// segfault is here, tho ????
// 	{
//         last_token = NULL;
//         return (str);
//     }
// 	str[index] = '\0';
// 	last_token = &str[index + 1];
// 	return (str);
// }

char *str_dup_until_index(char *str, int index)
{
	char *str_dup;
	int i;

	if (index == 0)
		return (ft_strdup("\0"));
	str_dup = (char *)malloc(sizeof(char) * (index + 2));
	if (!str_dup)
		exit(1);
	i = 0;
	while (str[i] != '\0' && i != index)
	{
		str_dup[i] = str[i];
		i++;
	}
	str_dup[i] = '\0';
	return (str_dup);
}

void	concat_path(char *buffer, const char *dir, const char *cmd)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (!cmd)
		return ;
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

char	*ft_strjoin_with_frees(char *s1, char *s2)
{
	size_t	len;
	char	*ptr;

	if (s1 == NULL)
	{
		ptr = ft_strdup(s2);
		if (s2)
			free(s2);
		return (ptr);
	}
	len = ft_strlen(s1) + ft_strlen(s2);
	ptr = malloc(len + 1);
	if (!ptr)
		return (NULL);
	ft_strlcpy(ptr, s1, len + 1);
	ft_strlcat(ptr, s2, len + 1);
	if (s1)
		free(s1);
	s1 = NULL;
	if (s2)
		free(s2);
	s2 = NULL;
	return (ptr);
}
