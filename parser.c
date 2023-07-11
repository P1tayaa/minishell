/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sboulain <sboulain@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 13:30:47 by sboulain          #+#    #+#             */
/*   Updated: 2023/07/11 16:20:56 by sboulain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	maybe add
	include parser.h
*/

typedef struct s_lexer
{
	char *tokenid;
	char *content;
	int	possition;
	
}	t_lexer;

int		count_pipes(char *str);
char	**get_list_of_tokenid(void);
void	free_list_of_tokenid(char **list_of_tokenid);
int	count_token(char *input);

t_data_table	*parsse_things(char *str)
{
	// int	token_num;

	printf("%d\n", count_token(str));
	// * old stuff
	// int				i;
	// t_data_table	*table;
	// 
	// table = malloc(sizeof(t_data_table *));
	// table->number_pip = count_pipes(str);
	// table->list_of_commands = ft_split(str, '|');

	// i = 0;
	// while (table->list_of_commands[i] != NULL)
	// {
	// 	printf("%s\n", table->list_of_commands[i]);
	// 	i++;
	// }
	return (NULL);
}
// I think this is lower then n^2 complexity, log(n) I think
int	count_token(char *input)
{
	char **list_of_tokenid;
	int	num_of_token;
	int	i;
	int	j;
	int	k;

	list_of_tokenid = get_list_of_tokenid();
	num_of_token = 0;
	i = 0;
	while (input[i] != '\0')
	{
		j = 0;
		while (list_of_tokenid[j] != NULL)
		{
			k = 0;
			while (list_of_tokenid[j][k] == input[i + k])
			{
				k++;
			}
			if (list_of_tokenid[j][k] == '\0')
			{
				i = i + k;
				printf(" i = %d, list_of_tokenid %s\n",i , list_of_tokenid[j]);
				num_of_token++;
				break ;
			}
			j++;
		}
		i++;
	}
	free_list_of_tokenid(list_of_tokenid);
	printf("test\n");
	return (num_of_token);
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

char **get_list_of_tokenid(void)
{
	char **list_of_tokenid;

	list_of_tokenid = (char **)malloc(sizeof(char) * 7);
	list_of_tokenid[0] = ft_strdup("||");
	list_of_tokenid[1] = ft_strdup("|");
	list_of_tokenid[2] = ft_strdup("<<");
	list_of_tokenid[3] = ft_strdup("<");
	list_of_tokenid[4] = ft_strdup(">>");
	list_of_tokenid[5] = ft_strdup(">");
	list_of_tokenid[6] = NULL;
	return (list_of_tokenid);
}

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
