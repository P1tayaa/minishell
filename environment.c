/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 12:56:41 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/01 01:31:32 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_with_frees(char *s1, char *s2);
void	free_double_array(char **list_of_tokenid);

char	*malloc_and_write_line(char *copy, char *og)
{
	int		i;

	i = 0;
	while (og[i])
		i++;
	copy = (char *) malloc((i + 1) * sizeof(char));
	i = 0;
	while (og[i] != '\0')
	{
		copy[i] = og[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

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
    char *temp;

	temp = *str1;
    *str1 = *str2;
    *str2 = temp;
}


void	recursiveBubble(char **environment, int string_count, int loop, int index)
{
	if (loop == string_count - 1)
		return;  // Base condition for outer recursion
	if (index == string_count - loop - 1)
		recursiveBubble(environment, string_count, loop + 1, 0);		// // If we've reached the end of the inner loop for this pass, move to the next pass
	else
	{
		if (custom_strcmp(environment[index], environment[index + 1]) > 0)
			swap(&environment[index], &environment[index + 1]);
		recursiveBubble(environment, string_count, loop, index + 1);
    }
}

void	printArray(char **environment, int string_count, int idx)
{
	if (idx == string_count)
		return;

	printf("declare -x %s\n", environment[idx]);
	printArray(environment, string_count, idx + 1);
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
	recursiveBubble(sorted_environment, i, 0, 0);
	printArray(sorted_environment, count, 0);
	free_double_array(sorted_environment);
}

// frees name and value
void	set_env(char *name, char *value, char ***environment)
{
	int		i;
	char	*to_set;
	size_t	value_len;
	char	**new_environment;

	i = 0;
    value_len = ft_strlen(value);
	while ((*environment)[i])
	{
		if (ft_memcmp((*environment)[i], name, ft_strlen(name)) == 0)
		{
			if (value_len == 0)
			{
				puts("should break");
				break ;
			}
			free((*environment)[i]);
			to_set = ft_strjoin_with_frees(ft_strjoin_with_frees(name, ft_strdup("=")), value);
			(*environment)[i] = to_set;
			return ;
		}
		i++;
	}
	printf("i = %i\n", i);
	if ((*environment)[i] == NULL)
	{
		new_environment = (char **) ft_realloc((*environment), (i + 2) * sizeof(char *));
		if (value_len == 0)
		{
			puts("should see me");
			to_set = ft_strdup(name);
		}
		else
			to_set = ft_strjoin_with_frees(ft_strjoin_with_frees(name, ft_strdup("=")), value);
		free((*environment));
		(*environment) = new_environment;
		(*environment)[i] = to_set;
		(*environment)[i + 1] = NULL;
	}
}

int	unset_env(char *name, char ***environment)
{
	int	i;
	int	j;

	i = 0;
	puts("unsetting");
	while ((*environment)[i])
	{
		if (strncmp((*environment)[i], name, strlen(name)) == 0)
		{
			free((*environment)[i]);
			j = i;
			while ((*environment)[j+1])
			{
				(*environment)[j] = (*environment)[j+1];
				j++;
			}
			(*environment)[j] = NULL;
			return (0);
		}
		i++;
	}
	return (-1);
}

void	print_export(char ***environment)
{
	int	i;

	i = 0;
	while ((*environment)[i] != NULL)
	{
		printf("declare -x %s\n", (*environment)[i]);
		i++;
	}
}

void	print_env(char ***environment)
{
	int	i;

	i = 0;
	while ((*environment)[i])
	{
		if (ft_strchr((*environment)[i], '=') != 0)
			printf("%s\n", (*environment)[i]);
		i++;
	}
}

// env cherche l'égalle, export all values
char	***get_env(void)
{
	int			i;
	int			count;
	extern char **environ;
	static char	**environment = NULL;
	char 		***copy;

	count = 0;
	i = 0;
	if (environment == NULL)
	{
		while (environ[count] != NULL)
			count++;
		environment = (char **) malloc((count + 1) * sizeof(char *));
		if (!environment)
			exit(EXIT_FAILURE);
		while (environ[i] != NULL)
		{
			environment[i] = malloc_and_write_line(environment[i], environ[i]);
			i++;
		}
		environment[i] = NULL;
	}
	copy = &environment;
	return (copy);
}
