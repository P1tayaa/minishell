/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_help.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sboulain <sboulain@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 12:36:35 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/09/18 15:31:17 by sboulain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_lexer(t_lexer **lexer)
{
	int	i;

	i = 0;
	if (!lexer)
		return ;
	while (lexer[i] != NULL)
	{
		if (lexer[i] != NULL)
		{
			if (lexer[i]->args != NULL)
				free(lexer[i]->args);
			if (lexer[i]->cmd != NULL)
				free(lexer[i]->cmd);
			if (lexer[i]->flags != NULL)
				free(lexer[i]->flags);
			free(lexer[i]);
		}
		i++;
	}
	free(lexer);
}


/*
	makes (malloced) a array of strings that contains all tokenid and ends with NULL.
	tokenid: ||, |, <<, <, >>, >
*/
char **get_list_of_tokenid(void)
{
	char **list_of_tokenid;

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
void	free_list_of_tokenid(char **list_of_tokenid)
{
	int	i;

	i = 0;
	while (list_of_tokenid[i] != NULL)
	{
		free(list_of_tokenid[i]);
		i++;
	}
	if (list_of_tokenid)
		free(list_of_tokenid);
}
