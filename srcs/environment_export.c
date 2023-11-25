/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_export.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 21:39:16 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 21:48:09 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	custom_strcmp(const char *str1, const char *str2)
{
	while (*str1 && (*str1 == *str2))
	{
		str1++;
		str2++;
	}
	return (*(unsigned char *)str1 - *(unsigned char *)str2);
}

void	swap(char **str1, char **str2)
{
	char	*temp;

	temp = *str1;
	*str1 = *str2;
	*str2 = temp;
}

void	recursive_bubble(char **environment, int string_count,
		int loop, int index)
{
	if (loop == string_count - 1)
		return ;
	if (index == string_count - loop - 1)
		recursive_bubble(environment, string_count, loop + 1, 0);
	else
	{
		if (custom_strcmp(environment[index], environment[index + 1]) > 0)
			swap(&environment[index], &environment[index + 1]);
		recursive_bubble(environment, string_count, loop, index + 1);
	}
}

void	print_array(char **environment, int string_count, int idx)
{
	if (idx == string_count)
		return ;
	printf("declare -x %s\n", environment[idx]);
	print_array(environment, string_count, idx + 1);
}

void	ascii_sort(char **environment)
{
	int		count;
	char	**sorted_environment;
	int		i;

	i = 0;
	count = 0;
	while (environment[count] != NULL)
		count++;
	sorted_environment = (char **)malloc(sizeof(char *) * (count + 1));
	while (i < count)
	{
		sorted_environment[i] = (char *)malloc(ft_strlen(environment[i]) + 1);
		ft_strcpy(sorted_environment[i], environment[i]);
		i++;
	}
	sorted_environment[i] = NULL;
	recursive_bubble(sorted_environment, i, 0, 0);
	print_array(sorted_environment, count, 0);
	free_double_array(sorted_environment);
}
