/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_local_function.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 14:23:41 by marvin            #+#    #+#             */
/*   Updated: 2023/06/14 14:23:41 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **get_all_paths(void);

char *execute_local_function(char **argv)
{
	char	**all_paths;

	all_paths = get_all_paths();
}

char **get_all_paths(void)
{
	char	**all_paths;
	extern char	*PATH;

	all_paths = ft_split(PATH, ':');
}