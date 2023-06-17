/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omathot <omathot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 13:30:47 by sboulain          #+#    #+#             */
/*   Updated: 2023/06/17 16:14:15 by omathot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_pipes(char *str);

t_data_table	*parsse_things(char *str)
{
	// int				i;
	t_data_table	*table;

	table = malloc(sizeof(t_data_table *));
	table->number_pip = count_pipes(str);
	table->list_of_commands = ft_split(str, '|');

	// i = 0;
	// while (table->list_of_commands[i] != NULL)
	// {
	// 	printf("%s\n", table->list_of_commands[i]);
	// 	i++;
	// }
	return (table);
}

int	count_pipes(char *str)
{
	int	i;
	int	number_of_pipes;

	i = 0;
	number_of_pipes = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '|')
			number_of_pipes++;
		i++;
	}
	return (number_of_pipes);
}
