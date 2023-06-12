/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omathot <omathot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 11:53:41 by omathot           #+#    #+#             */
/*   Updated: 2023/06/12 14:07:00 by omathot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Handle ’ (single quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence.
Handle " (double quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence except for $ (dollar sign).
*/

int	*get_doll_position(char *str);

// here *str is &str[3] from main.
void	check_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		// 39 is ascii value for single quotes BUT, \ is in front of it
		if (str[0] == '\'' && str[ft_strlen(str) - 1] == '\'')
			printf("%s\n", str);
		// 34 is ascii value for double quotes,
		if (str[0] == '\"' && str[ft_strlen(str) - 1] == '\"')
		{
			getenv()
			printf("")
		}
	}
}


char	*handle_expand_doll(char *str)
{
	int	*doll_pos;

	doll_pos = get_doll_position(str);
	getenv(str);
}

// les arico $ARG song $ARG2

// les arico 1 2 3 4 song 

int	*get_doll_position(char *str)
{
	int	i;
	int	num_doll;
	int	*doll_pos;

	i = 0;
	num_doll = 0;
	while (str[i])
	{
		if (str[i] == '$')
			num_doll++;
		i++;
	}
	doll_pos = malloc(sizeof(int) * num_doll)
	if (!doll_pos)
		exit(EXIT_FAILURE);
	i = 0;
	num_doll = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			doll_pos[num_doll] = i;
			num_doll++;
		}
		i++;
	}
}

char **all;
malloc (sizeof(char *) * parts);
all[0] = strconais;
all[1] = getenv(str);

char *finish;
finish(len2char())

1. look at first open - pattern matching     'asudgadagg"ausydgasdas"adhad';
2. getenv 