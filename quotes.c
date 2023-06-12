/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omathot <omathot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 11:53:41 by omathot           #+#    #+#             */
/*   Updated: 2023/06/12 11:57:42 by omathot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Handle ’ (single quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence.
Handle " (double quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence except for $ (dollar sign).
*/

// here *str is &str[3] from main.
void	check_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		// 39 is ascii value for single quotes BUT, \ is in front of it
		if (str[0] == '\'' && str[ft_strlen(str) - 1] == '\'')
			ignore metadata;
		// 34 is ascii value for double quotes,
		if (str[0] == "\"" && str[ft_strlen(str) - 1] == "\"")
			ignore metadata except $;
	}
}
