/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 17:01:53 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 23:12:31 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		ft_char_find(char *str, const char *list_of_char);
bool	ft_isspace(unsigned char c);

int	ft_strtok_line_help(
	char **str_reminder, int *index_unter_delim, int *curser)
{
	(*index_unter_delim) = ft_strlen(&(*str_reminder)[(*curser)]);
	return (1);
}

char	*get_string_at_index(char *string, int index)
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

int	ft_char_find(char *str, const char *list_of_char)
{
	int	i;
	int	j;

	if (str == NULL)
		return (-1);
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

char	*str_dup_until_index(char *str, int index)
{
	char	*str_dup;
	int		i;

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

// void	test_export_for_main(t_lexer **lexer)
// {
// 	char	**export_content;
// 	int		i;

// 	if (is_str_export(lexer[0]->args) == true)
// 	{
// 		export_content = get_export_var(lexer[0]->args);
// 		if (export_content == NULL)
// 			printf("error\n");
// 		else
// 		{
// 			i = 0;
// 			while (export_content[i] != NULL)
// 				i++;
// 		}
// 	}
// 	pause();
// }