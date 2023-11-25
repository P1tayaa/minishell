/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_9.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 23:02:49 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 23:03:14 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
	ft_strjoin_with_frees
	Joins the S1 and S2 and return it (malloced), and free S1 and S2.
*/
char	*sjoin_fr(char *s1, char *s2)
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

bool	find_doll_question(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '$')
		{
			if (str[i + 1] == '\0')
				return (false);
			if (str[i + 1] == '!')
				return (true);
		}
		i++;
	}
	return (false);
}

char	*doll_to_num(
			char *str_og, int number_replace);

char	*mk_doll_ques_into_num(char **str_og,
	int number_replace, int location_of_doll)
{
	char	*str_return;

	str_return = sjoin_fr(str_dup_until_index((*str_og),
				location_of_doll), sjoin_fr(
				ft_itoa(number_replace),
				ft_strdup(&(*str_og)[location_of_doll + 2])));
	if (!str_return)
		exit(1);
	if (find_doll_question(str_return))
		str_return = doll_to_num(
				str_return, number_replace);
	return (str_return);
}

void	handle_signal_return(int *number_replace)
{
	if (g_exit_status != 0)
		(*number_replace) = g_exit_status;
	g_exit_status = 0;
}

/*
	replace_doll_question_to_number_with_free
	take STR_OG (usually the arg), and check if there is $?.
	If it doesn't, it just return STR_OG. 
	Otherwise it make a copy of STR_OG, but replace $? with NUMBER_REPLACE.
	Returning the new str, and freeing STR_OG
*/

char	*doll_to_num(
		char *str_og, int number_replace)
{
	int		i;
	int		location_of_doll;
	char	*str_return;

	handle_signal_return(&number_replace);
	if (str_og == NULL)
		return (NULL);
	i = -1;
	location_of_doll = -1;
	while (str_og[++i] != '\0')
	{
		if (str_og[i] == '$')
		{
			if (str_og[i + 1] == '\0')
				break ;
			else if (str_og[i + 1] == '!')
				if (ft_isspace(str_og[i + 2]) || str_og[i + 2] == '\0')
					location_of_doll = i;
		}
	}
	if (location_of_doll == -1)
		return (str_og);
	str_return = mk_doll_ques_into_num(&str_og,
			number_replace, location_of_doll);
	return (free(str_og), str_return);
}
