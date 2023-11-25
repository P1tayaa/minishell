/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_7.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 23:01:56 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 23:02:03 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strdup_until_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '=' && str[i] != '\0')
		i++;
	return (str_dup_until_index(str, i));
}

int	get_num_export_con(char *arg)
{
	int		i;
	bool	is_space;
	int		num_var;

	i = 0;
	is_space = false;
	num_var = 1;
	while (arg[i] != '\0')
	{
		if (!is_space && !ft_isspace(arg[i]))
		{
			num_var++;
			is_space = true;
		}
		else
			if (ft_isspace(arg[i]))
				is_space = false;
		i++;
	}
	return (num_var * 2);
}

bool	ft_isspace(unsigned char c);

char	**get_word_one_by_one(char *str)
{
	char	**all_world;
	int		i;
	int		i_all_world;

	all_world = malloc(sizeof(char *) * ((get_num_export_con(str) * 2) + 1));
	i = 0;
	i_all_world = 0;
	while (str[i] != '\0')
	{
		while (ft_isspace(str[i]))
			i++;
		all_world[i_all_world] = ft_strdup_until_space(&str[i]);
		if (str[i + ft_strlen(all_world[i_all_world])] == '\0')
		{
			i_all_world++;
			break ;
		}
		i = i + ft_strlen(all_world[i_all_world]) + 1;
		i_all_world++;
	}
	all_world[i_all_world] = NULL;
	return (all_world);
}

/*
og vars
char	**work_split;
char	**export_content;
int		i_export_content;
int		i_work_split;

*/

char	**get_export_var(char *arg_of_export)
{
	char	**work_split;
	char	**export_content;
	int		i_c;
	int		i_w;

	i_c = 0;
	i_w = 0;
	export_content = malloc(sizeof(char *)
			* ((get_num_export_con(arg_of_export) * 2) + 3));
	work_split = get_word_one_by_one(arg_of_export);
	i_w = 0;
	while (work_split[i_w] != NULL)
	{
		export_content[i_c] = ft_strdup_until_equal(work_split[i_w]);
		i_c++;
		if (ft_strlen(export_content[i_c - 1]) == ft_strlen(work_split[i_w]))
			export_content[i_c] = ft_strdup("\0");
		else
			export_content[i_c] = ft_strdup(&((work_split[i_w])
					[ft_strlen(export_content[i_c - 1]) + 1]));
		i_c++;
		free(work_split[i_w++]);
	}
	export_content[i_c] = NULL;
	return (free(work_split), export_content);
}

void	export_content_freeur(char ***export_content)
{
	int	i;

	i = 0;
	while ((*export_content)[i] != NULL)
	{
		free((*export_content)[i]);
		i++;
	}
	free((*export_content));
}
