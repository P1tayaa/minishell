/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 12:56:41 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/10/29 16:14:46 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_with_frees(char *s1, char *s2);

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


// frees name and value
void	set_env(char *name, char *value, char ***environment)
{
	int		i;
	char	*to_set;
	size_t	name_len; 
	size_t	value_len;
	char	**new_environment;

	i = 0;
    name_len = ft_strlen(name);
    value_len = ft_strlen(value);
	while ((*environment)[i])
	{
		if (ft_memcmp((*environment)[i], name, ft_strlen(name)) == 0 && (*environment)[i][ft_strlen(name)] == '=')
		{
			free((*environment)[i]);
			(*environment)[i] = NULL;
			break;
		}
		i++;
	}
	to_set = (char *) malloc(name_len + value_len + 2);  // +2 for '=' and '\0'
	if (!to_set)
		exit(EXIT_FAILURE);
	// if it has a value, add the equal, else do not put the equal (important so we don't print in env)
	if (name[ft_strlen(name)] == '=')
		to_set = ft_strjoin_with_frees(ft_strjoin_with_frees(name, ft_strdup("=")), value);
	else
		to_set = ft_strdup(name);
	if ((*environment)[i])
		(*environment)[i] = to_set;
	else
	{
		puts("should appear");
		new_environment = (char **) realloc((*environment), (i + 2) * sizeof(char *));
		if (!new_environment)
		{
			free(to_set);
			exit(EXIT_FAILURE);
		}
		(*environment) = new_environment;
		(*environment)[i] = to_set;
		(*environment)[i + 1] = NULL;  // Terminate the array
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
	while ((*environment)[i])
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
