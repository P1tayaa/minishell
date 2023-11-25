/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 12:56:41 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 21:48:08 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
