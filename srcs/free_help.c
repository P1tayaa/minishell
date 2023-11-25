/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_help.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 12:36:35 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 21:48:05 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
	makes (malloced) a array of strings
	that contains all tokenid and ends with NULL.
	tokenid: ||, |, <<, <, >>, >
*/
char	**get_list_of_tokenid(void)
{
	char	**list_of_tokenid;

	list_of_tokenid = (char **)malloc(sizeof(char *) * 7);
	if (!list_of_tokenid)
		exit(1);
	list_of_tokenid[0] = ft_strdup("||\0");
	list_of_tokenid[1] = ft_strdup("|\0");
	list_of_tokenid[2] = ft_strdup("<<\0");
	list_of_tokenid[3] = ft_strdup("<\0");
	list_of_tokenid[4] = ft_strdup(">>\0");
	list_of_tokenid[5] = ft_strdup(">\0");
	if (!list_of_tokenid[0] || !list_of_tokenid[1] || !list_of_tokenid[2]
		|| !list_of_tokenid[3] || !list_of_tokenid[4] || !list_of_tokenid[5])
		exit(1);
	list_of_tokenid[6] = NULL;
	return (list_of_tokenid);
}

/*
	frees the list of tokenid, made with get_list_of_tokenid().
*/
void	free_double_array(char **list_of_tokenid)
{
	int	i;

	i = 0;
	if (list_of_tokenid == NULL)
		return ;
	while (list_of_tokenid[i] != NULL)
	{
		free(list_of_tokenid[i]);
		i++;
	}
	if (list_of_tokenid)
		free(list_of_tokenid);
}
