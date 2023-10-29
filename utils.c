/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 17:01:53 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/10/28 11:44:53 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_char_find(char *str, const char *list_of_char);
bool	ft_isspace(unsigned char c);

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
/*
	Joins the S1 and S2 and return it (malloced), and free S1 and S2.
*/
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
/*
	take STR_OG (usually the arg), and check if there is $?.
	If it doesn't, it just return STR_OG. 
	Otherwise it make a copy of STR_OG, but replace $? with NUMBER_REPLACE.
	Returning the new str, and freeing STR_OG
*/
char *replace_doll_question_to_number_with_free(char *str_og, int number_replace)
{
	int	i;
	int	location_of_doll;
	char *str_return;

	if (str_og == NULL)
		return NULL;
	i = 0;
	location_of_doll = -1;
	while (str_og[i] != '\0')
	{
		if (str_og[i] == '$')
		{
			if (str_og[i + 1] == '\0')
				break ;
			else if (str_og[i + 1] == '!')
				if (ft_isspace(str_og[i + 2]) || str_og[i + 2] == '\0')
					location_of_doll = i;
		}
		i++;
	}
	if (location_of_doll == -1)
		return (str_og);
	str_return = ft_strjoin_with_frees(str_dup_until_index(str_og, location_of_doll), ft_strjoin_with_frees(ft_itoa(number_replace), ft_strdup(&str_og[location_of_doll + 2])));
	if (!str_return)
		exit(1);
	free(str_og);
	return (str_return);
}

int return_biggest_int(int a, int b)
{
    if (a > b)
        return (a);
    return (b);
}

char *here_doc_starter(char *wordlocking_for)
{
    // int i;
    char *str_return;
    char *read_line_str;
    
    str_return = NULL;
    read_line_str = readline(" > ");
    while (ft_strncmp(wordlocking_for, read_line_str, return_biggest_int(ft_strlen(wordlocking_for), ft_strlen(read_line_str)) )!= 0)
    {
        str_return = ft_strjoin_with_frees(str_return, ft_strjoin("\n", read_line_str));
        read_line_str = readline(" > ");
    }
	str_return = ft_strjoin_with_frees(str_return, ft_strdup("\n"));
    return (str_return);
}