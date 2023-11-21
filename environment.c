/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 12:56:41 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/20 14:54:08 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*sjoin_fr(char *s1, char *s2);
void	free_double_array(char **list_of_tokenid);
int		ft_char_find(char *str, const char *list_of_char);

char	*malloc_and_write_line(char *copy, char *og)
{
	int	i;

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

char	*check_exisisting_env(char ***environment, char *name,
		char **value, int *i_og)
{
	size_t	value_len;
	char	*to_set;
	int		i;

	i = -1;
	value_len = ft_strlen((*value));
	to_set = NULL;
	while ((*environment)[++i] != NULL)
	{
		if (ft_memcmp((*environment)[i], name, ft_strlen(name)) == 0)
		{
			if (value_len == 0)
			{
				(*i_og) = i;
				return (free(name), free((*value)), to_set);
			}
			free((*environment)[i]);
			to_set = sjoin_fr(sjoin_fr(name, ft_strdup("=")), (*value));
			(*environment)[i] = to_set;
			(*i_og) = i;
			return (NULL);
		}
	}
	(*i_og) = i;
	return (to_set);
}

// frees name and value
void	set_env(char *name, char *value, char ***environment)
{
	int		i;
	char	*to_set;
	char	**new_environment;

	i = 0;
	to_set = check_exisisting_env(environment, name, &value, &i);
	if (to_set == NULL && (*environment)[i] != NULL)
		return ;
	if ((*environment)[i] == NULL)
	{
		new_environment = (char **) ft_realloc((*environment),
				(i + 2) * sizeof(char *));
		if (ft_strlen(value) == 0)
		{
			to_set = ft_strdup(name);
			free(value);
			free(name);
		}
		else
			to_set = sjoin_fr(sjoin_fr(name, ft_strdup("=")), value);
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
	while ((*environment)[i])
	{
		if (strncmp((*environment)[i], name, strlen(name)) == 0)
		{
			free((*environment)[i]);
			j = i;
			while ((*environment)[j + 1])
			{
				(*environment)[j] = (*environment)[j + 1];
				j++;
			}
			(*environment)[j] = NULL;
			return (0);
		}
		i++;
	}
	return (-1);
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
	extern char	**environ;
	static char	**environment;
	char		***copy;

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

char	*get_env_of_valus_str(char *str)
{
	int		i;
	char	***env;

	env = get_env();
	i = 0;
	while (((*env)[i] != NULL))
	{
		if (ft_memcmp((*env)[i], str, ft_strlen(str)) == 0)
			return (ft_strdup(&(*env)[i][ft_char_find((*env)[i], "=") + 1]));
		i++;
	}
	return (NULL);
}
