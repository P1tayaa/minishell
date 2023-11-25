/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_flags.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 22:03:24 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 22:03:39 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*init_new_flag_str(int flags_char_len, char **str, int num_of_dash)
{
	char	*new_str;

	if (ft_strlen((*str)) - (sizeof(char) * (
				flags_char_len + num_of_dash)) != 0)
	{
		new_str = malloc(ft_strlen((*str)) - (sizeof(char)
					* (flags_char_len + num_of_dash)) + 1);
		if (!new_str)
			exit(1);
	}
	else
		new_str = NULL;
	return (new_str);
}

void	set_value_of_flag_str_p2(char **new_str, char **str, int *k, int *i)
{
	(*new_str)[(*k)] = (*str)[(*i)];
	if ((*str)[(*i) + 1] != '\0')
		if (((*str)[(*i) + 1]))
			(*k)++;
	(*i)++;
}

void	place_flag(int *i, char **str, char **flags, int *flags_char_len)
{
	int	j;

	j = 0;
	(*i)++;
	while ((*str)[(*i) + j] != '\0' && ft_isspace((*str)[(*i) + j]) == false)
	{
		(*flags)[(*flags_char_len)] = (*str)[(*i) + j];
		j++;
		(*flags_char_len)++;
	}
	(*i) = (*i) + j;
}

// set the values of string, but doesn't add the '-'
int	set_value_of_flag_str(int *flags_char_len,
		char **str, char	**flags, char **new_str)
{
	int	i;
	int	k;

	i = 0;
	(*flags_char_len) = 0;
	k = 0;
	while ((*str)[i] != '\0')
	{
		if (ft_isspace((*str)[i]) == true || i == 0)
		{
			if (ft_isspace((*str)[i]))
				i++;
			if ((*str)[i] == '-')
			{
				place_flag(&i, str, flags, flags_char_len);
				continue ;
			}
			if ((*str)[i] == '\0')
				break ;
		}
		set_value_of_flag_str_p2(new_str, str, &k, &i);
	}
	return (k);
}

// null terminate all right string or make them null if they are.
void	null_terminate_flags_str(int flags_char_len,
		char **new_str, char **str, char **flags)
{
	int	k;

	k = set_value_of_flag_str(&flags_char_len, str, flags, new_str);
	if (k == 0)
	{
		free((*new_str));
		(*new_str) = NULL;
	}
	if ((*new_str) != NULL)
		(*new_str)[k] = '\0';
	free((*str));
	(*str) = (*new_str);
	(*flags)[flags_char_len] = '\0';
}
