/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 22:22:45 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 22:56:16 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	ft_isspace(unsigned char c)
{
	if (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
		|| c == ' ')
		return (true);
	return (false);
}

void	prep_all_var(t_post_quotes ***content,
		int *all_var_rm_total, char ***all_var_rm, int *i)
{
	(*all_var_rm_total) = (*i) * 2 + 1;
	(*all_var_rm) = malloc(sizeof(char *) * ((*all_var_rm_total)));
	if (is_str_unset((*content)[0]->content))
		(*i) = 1;
	else
		(*i) = 0;
}

int	unset_quotes_nq_else(t_post_quotes **content, char ***all_var_rm,
		int *all_var_rm_total, int *all_var_rm_num)
{
	char	**all_var_rm_temp;
	int		j;

	if (is_all_space((*content)->content))
		return (0);
	all_var_rm_temp = split_spaces((*content)->content);
	j = 0;
	while (all_var_rm_temp[j] != NULL)
		j++;
	if (j == 1)
	{
		(*all_var_rm)[(*all_var_rm_num)++] = ft_strdup(all_var_rm_temp[0]);
		free(all_var_rm_temp[0]);
		return (free(all_var_rm_temp), 0);
	}
	(*all_var_rm)[(*all_var_rm_num)] = NULL;
	(*all_var_rm_total) = (*all_var_rm_total) + j;
	(*all_var_rm) = remalloc_and_dup((*all_var_rm), (*all_var_rm_total));
	j = 0;
	while (all_var_rm_temp[j] != NULL)
		(*all_var_rm)[(*all_var_rm_num)++] = all_var_rm_temp[j++];
	return (free(all_var_rm_temp), 0);
}

void	finish_check_unset(char ***all_var_rm,
		t_post_quotes ***content, t_lexer ***lexer)
{
	int	i;

	i = 0;
	while ((*all_var_rm)[i] != NULL)
	{
		unset_env((*all_var_rm)[i], get_env());
		i++;
	}
	i = 0;
	while ((*all_var_rm)[i] != NULL)
	{
		free((*all_var_rm)[i]);
		i++;
	}
	free((*all_var_rm));
	free_content(*content);
	lexer_free((*lexer));
}

bool	check_unset_for_quotes(t_post_quotes ***content, t_lexer ***lexer)
{
	char	**all_var_rm;
	int		all_var_rm_num;
	int		all_var_rm_total;
	int		i;

	if (is_str_unset((*lexer)[0]->cmd) == false)
		return (false);
	if (checks_andset_i(lexer, content, &i) == 1)
		return (true);
	prep_all_var(content, &all_var_rm_total, &all_var_rm, &i);
	all_var_rm_num = 0;
	while ((*content)[i])
	{
		if ((*content)[i]->is_quotes == true)
			all_var_rm[all_var_rm_num] = ft_strdup((*content)[i++]->content);
		else
		{
			unset_quotes_nq_else(&((*content)[i++]),
				&all_var_rm, &all_var_rm_total, &all_var_rm_num);
			continue ;
		}
		all_var_rm_num++;
	}
	all_var_rm[all_var_rm_num] = NULL;
	return (finish_check_unset(&all_var_rm, content, lexer), true);
}
