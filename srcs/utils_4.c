/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 22:57:43 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 22:57:49 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_len_split_world(char *str)
{
	int		i;
	int		word_count;

	i = 0;
	word_count = 0;
	while (str[i] != '\0')
	{
		while (ft_isspace((str[i])) && str[i])
			i++;
		if (!(ft_isspace(str[i])) && str[i])
		{
			word_count++;
			i++;
		}
	}
	return (word_count);
}

int		count_len_split_world(char *str);
int		count_char_unti_space(char *str);

// char	**initialize_str_split(char *str)
// {
// 	char	**str_split;

// 	str_split = (char **)
// 		malloc(sizeof(char *)
// 			* (count_len_split_world(str) + 1));
// 	if (!str_split)
// 		exit(-1);
// 	return (str_split);
// }

void	finish_writing(int *j, int *word_count, char **str_split, bool *writing)
{
	str_split[(*word_count)++][(*j)] = '\0';
	(*j) = 0;
	*writing = false;
}

char	*remove_front_spaces(char *str);

// char	**finalize_str_split(char **str, char **str_split, int word_count)
// {
// 	int	j;

// 	j = 0;
// 	(*str) = remove_front_spaces((*str));
// 	while ((*str)[j] && !ft_isspace((*str)[j]))
// 		j++;
// 	if (!ft_isspace((*str)[j - 1]))
// 		str_split[word_count][j] = '\0';
// 	str_split[++word_count] = NULL;
// 	return (str_split);
// }

bool	writing_process(bool *writing,
		char ***str_split, int *word_count, int *j)
{
	if ((*writing))
	{
		(*str_split)[(*word_count)][(*j)] = '\0';
		(*word_count)++;
		(*j) = 0;
		(*writing) = false;
	}
	return (1);
}

int	cc_u_s(char *str)
{
	return (count_char_unti_space(str));
}

int	finish_or_malloc_word(char *str,
		char ***s_p, int *wc)
{
	bool	writing;
	int		j;
	int		i;

	writing = false;
	j = 0;
	i = -1;
	while (str[++i] != '\0')
	{
		if (ft_isspace(str[i]))
		{
			if (str[1] != '\0' && writing_process(
					&writing, s_p, wc, &j))
				if (!ft_isspace(str[i + 1]) && str[i + 1] != '\0')
					(*s_p)[(*wc)] = malloc((cc_u_s(&str[i + 1]) + 1));
		}
		else
		{
			if (i == 0)
				(*s_p)[(*wc)] = malloc((cc_u_s(&str[i]) + 1));
			writing = true;
			(*s_p)[(*wc)][j++] = str[i];
		}
	}
	return (j);
}
