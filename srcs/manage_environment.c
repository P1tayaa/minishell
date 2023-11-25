/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_environment.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 21:41:54 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 21:48:02 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
