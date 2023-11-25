/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 22:24:52 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 22:24:59 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strdup_intil_index_n(char *s1, int n)
{
	int		i;
	char	*c;

	i = 0;
	c = malloc(sizeof(char) * (n + 2));
	if (!c)
		return (NULL);
	while (i <= n)
	{
		c[i] = s1[i];
		i++;
	}
	c[i] = '\0';
	return (c);
}

bool				ft_isspace(unsigned char c);

int	find_next_space(char *str)
{
	int	i;

	i = 0;
	while (!ft_isspace(str[i]))
	{
		if (str[i] == '\0')
			return (-1);
		i++;
	}
	return (i);
}

int	find_next_quote(char *str)
{
	int	i;

	i = 0;
	while (!(str[i] == '\"'))
	{
		if (str[i] == '\0')
			return (-1);
		i++;
	}
	return (i);
}

/*
char	*spit_text_args_even(char *str, int	*doll_pos,
		int num_doll, char *string_split)

*/
char	*spit_text_args_even(char *str, int	*d_p,
		int n_d, char **s_s)
{
	if (d_p[n_d] != -1)
	{
		if (find_next_space(&str[d_p[n_d - 1]] + 1) == -1 || find_next_space(
				&str[d_p[n_d - 1]] + 1) > d_p[n_d] - (d_p[n_d - 1] + 1))
			(*s_s) = ft_strdup("\0");
		else
			(*s_s) = ft_strdup_intil_index_n(&str[d_p[n_d - 1]
					+ find_next_space(&str[d_p[n_d - 1]])], d_p[n_d]
					- (d_p[n_d - 1] + find_next_space(&str[d_p[n_d - 1]])) - 1);
	}
	else
	{
		if (find_next_space(&str[d_p[n_d - 1]]) == -1)
		{
			if (find_next_quote(&str[d_p[n_d - 1]]) == -1)
				(*s_s) = ft_strdup("\0");
			else
				(*s_s) = ft_strdup("\"");
		}
		else
			(*s_s) = ft_strdup_intil_index_n(&str[d_p[n_d - 1]
					+ find_next_space(&str[d_p[n_d - 1]])], ft_strlen(&str[
						d_p[n_d - 1] + find_next_space(&str[d_p[n_d - 1]])]));
	}
	return ((*s_s));
}

void	spit_text_args_odd(char *str, int *doll_pos,
		int num_doll, char	**temp_str)
{
	if (find_next_space(&str[doll_pos[num_doll]]) == -1)
	{
		if (doll_pos[num_doll + 1] != -1)
			(*temp_str) = ft_strdup_intil_index_n(&str[doll_pos[
					num_doll] + 1], doll_pos[num_doll + 1]
					- (doll_pos[num_doll] + 2));
		else
		{
			if (find_next_quote(&str[doll_pos[num_doll] + 1]) - 1 != -2)
				(*temp_str) = ft_strdup_intil_index_n(&str[doll_pos
					[num_doll] + 1], find_next_quote(
							&str[doll_pos[num_doll] + 1]) - 1);
			else
				(*temp_str) = ft_strdup(&str[doll_pos[num_doll] + 1]);
		}
	}
	else if (doll_pos[num_doll + 1] != -1 && find_next_space(&str[
				doll_pos[num_doll]] + 1) > doll_pos[
			num_doll + 1] - (doll_pos[num_doll] + 1))
		(*temp_str) = ft_strdup_intil_index_n(&str[doll_pos
			[num_doll] + 1], doll_pos[num_doll + 1] - (doll_pos[num_doll] + 2));
	else
		(*temp_str) = ft_strdup_intil_index_n(&str[doll_pos
			[num_doll] + 1], find_next_space(&str[doll_pos[num_doll]] + 1) - 1);
}
