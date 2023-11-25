/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_spit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 22:24:13 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 22:45:19 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strdup_intil_index_n(char *s1, int n);
char	*spit_text_args_even(char *str, int	*d_p,
			int n_d, char **s_s);
void	spit_text_args_odd(char *str, int *doll_pos,
			int num_doll, char	**temp_str);

void	spit_text_args_odd_p2(char	**string_split, char **temp_str, int i)
{
	char	*temp;

	temp = get_env_of_valus_str((*temp_str));
	if (temp != NULL)
		string_split[i] = temp;
	else if ((*temp_str)[0] == '?' && (*temp_str)[1] == '\0')
	{
		string_split[i] = ft_strdup("$!");
	}
	else
	{
		string_split[i] = ft_strdup("");
	}
	free((*temp_str));
}

void	spit_text_args_init(int *num_doll, int *doll_pos,
		int *total_parts, char ***string_split)
{
	(*num_doll) = 0;
	while (doll_pos[(*num_doll)] != -1)
		(*num_doll)++;
	(*total_parts) = ((*num_doll) * 2) + 1;
	(*string_split) = malloc(sizeof(char *)
			* ((*total_parts) + 2));
	if (!(*string_split))
		exit(1);
}

void	terminate_string_split(char	***s_s, int i)
{
	(*s_s)[i] = NULL;
}

/*
	splite the STR into str and the dollars,
		and also replace the dollar with their value.
	Or for $? make it $! to be catch later. 
	And return the array of str.
	
	string_split = s_s
*/
char	**spit_text_args(char *str, int	*doll_pos)
{
	int		i;
	char	**s_s;
	int		num_doll;
	char	*temp_str;
	int		total_parts;

	spit_text_args_init(&num_doll, doll_pos, &total_parts, &s_s);
	i = -1;
	num_doll = 0;
	while (++i < (total_parts))
	{
		if (i % 2 == 0)
		{
			if (i == 0)
				s_s[i] = ft_strdup_intil_index_n(str, doll_pos[num_doll] - 1);
			else
				spit_text_args_even(str, doll_pos, num_doll, &(s_s[i]));
		}
		else
		{
			spit_text_args_odd(str, doll_pos, num_doll++, &temp_str);
			spit_text_args_odd_p2(s_s, &temp_str, i);
		}
	}
	return (terminate_string_split(&s_s, i), s_s);
}

int	*get_doll_position(char *str)
{
	int	i;
	int	num_doll;
	int	*doll_pos;

	i = 0;
	num_doll = 0;
	while (str[i] != '\0')
		if (str[i++] == '$')
			if (str[i] != '\0')
				if (str[i] != '=')
					num_doll++;
	doll_pos = malloc(sizeof(int) * (num_doll + 1));
	if (!doll_pos)
		exit(EXIT_FAILURE);
	i = -1;
	num_doll = 0;
	while (str[++i] != '\0')
	{
		if (str[i] == '$')
			if (str[i + 1] != '\0')
				if (str[i + 1] != '=')
					doll_pos[num_doll++] = i;
	}
	doll_pos[num_doll] = -1;
	return (doll_pos);
}
