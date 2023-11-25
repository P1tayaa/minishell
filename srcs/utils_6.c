/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_6.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 23:01:04 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 23:01:08 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	counte_num_new_var(t_post_quotes **content)
{
	int	i;
	int	num_of_vars;

	i = 0;
	num_of_vars = 0;
	while (content[i] != NULL)
	{
		if (content[i]->is_quotes)
		{
			num_of_vars++;
		}
		else
		{
			num_of_vars += get_num_export_con(content[i]->content);
		}
		i++;
	}
	return (num_of_vars * 2);
}

bool	is_all_space(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (!ft_isspace(str[i]))
			return (false);
		i++;
	}
	return (true);
}

bool	check_if_array_str_is_empty(char **array_str)
{
	int	i;

	if (array_str == NULL)
		return (true);
	i = 0;
	while (array_str[i])
	{
		if (array_str[i][0] != '\0')
			return (false);
		i++;
	}
	return (true);
}

char	get_last_char(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	i--;
	return (str[i]);
}

bool	check_export_for_quotes_check_start(
		t_lexer ***lexer, t_post_quotes	***content)
{
	if ((*lexer)[1] != NULL && (*lexer)[0]->args != NULL)
	{
		printf("Can't pipe export when setting a value\n");
		lexer_free((*lexer));
		free_content((*content));
		return (true);
	}
	if ((*lexer)[0]->args == NULL && (*lexer)[1] == NULL)
	{
		ascii_sort(*(get_env)());
		lexer_free((*lexer));
		free_content((*content));
		return (true);
	}
	if ((*lexer)[0]->args == NULL && (*lexer)[1] != NULL)
	{
		pipe_export(lexer);
		lexer_free((*lexer));
		free_content((*content));
		return (true);
	}
	return (false);
}
