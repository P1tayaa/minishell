/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_doll.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 22:27:22 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 22:44:02 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*rm_q_d_cpy_d_q(char **str,
			t_list_of_quotes *list_of_quotes,
			int *current_double_quotes_index, int *i);
char	*rm_q_d_cpy_s_q(char **str,
			t_list_of_quotes *list_of_quotes,
			int *current_single_quotes_index, int *i);
char	*rm_q_d_cpy_u_d_q(char **str,
			t_list_of_quotes *list_of_quotes,
			int *current_double_quotes_index, int *i);
char	*rm_q_d_cpy_u_s_q(char **str,
			t_list_of_quotes *list_of_quotes,
			int *current_single_quotes_index, int *i);

/*
	og var
	int		current_single_quotes_index;
	int		current_double_quotes_index;

*/
char	*remove_quotes_and_expand_dollars_p2(int i,
		char **str, t_list_of_quotes *l_of_q)
{
	int		c_s_q_i;
	int		c_d_q_i;
	char	*f_s;

	c_d_q_i = 0;
	c_s_q_i = 0;
	while ((*str)[i] != '\0')
	{
		if (i == l_of_q->double_quotes[c_d_q_i])
			f_s = sjoin_fr(f_s, rm_q_d_cpy_d_q(str, l_of_q, &c_d_q_i, &i));
		else if (i == l_of_q->single_quotes[c_s_q_i])
			f_s = sjoin_fr(f_s, rm_q_d_cpy_s_q(str, l_of_q, &c_d_q_i, &i));
		else if (l_of_q->double_quotes[c_d_q_i] != -1 && (l_of_q
				->double_quotes[c_d_q_i] < l_of_q->single_quotes[c_s_q_i]
				|| l_of_q->single_quotes[c_s_q_i] == -1))
			f_s = sjoin_fr(f_s, rm_q_d_cpy_u_d_q(str, l_of_q, &c_d_q_i, &i));
		else if (l_of_q->single_quotes[c_s_q_i] != -1 && (l_of_q
				->double_quotes[c_d_q_i] > l_of_q->single_quotes[c_s_q_i]
				|| l_of_q->double_quotes[c_d_q_i] == -1))
			f_s = sjoin_fr(f_s, rm_q_d_cpy_u_s_q(str, l_of_q, &c_d_q_i, &i));
		else if (l_of_q->double_quotes[c_d_q_i] == -1
			&& l_of_q->single_quotes[c_d_q_i] == -1)
			return (free(*str), sjoin_fr(f_s, ft_strdup(&(*str)[i])));
	}
	return (free(*str), f_s);
}

void	remove_quotes_and_expand_dollars(
		char **str, t_list_of_quotes *list_of_quotes)
{
	int		i;
	char	*final_str;

	i = 0;
	final_str = NULL;
	if (list_of_quotes->double_quotes[0] != 0
		&& list_of_quotes->single_quotes[0] != 0)
	{
		if (list_of_quotes->double_quotes[0] < list_of_quotes->single_quotes[0])
		{
			i = list_of_quotes->double_quotes[0];
			final_str = str_dup_until_index((*str),
					list_of_quotes->double_quotes[0] - 1);
		}
		else
		{
			i = list_of_quotes->single_quotes[0];
			final_str = str_dup_until_index((*str),
					list_of_quotes->single_quotes[0] - 1);
		}
		final_str = handle_expand_doll(final_str);
	}
	(*str) = remove_quotes_and_expand_dollars_p2(i, str, list_of_quotes);
}

// prints list of coordinates of quotes
void	print_coordines_of_all_quotes(t_list_of_quotes *list_of_quotes)
{
	int	i;

	i = 0;
	while (list_of_quotes->double_quotes[i] != -1)
	{
		printf("double quotes %d at incex %d\n", i,
			list_of_quotes->double_quotes[i]);
		i++;
	}
	i = 0;
	while (list_of_quotes->single_quotes[i] != -1)
	{
		printf("single quotes %d at incex %d\n", i,
			list_of_quotes->single_quotes[i]);
		i++;
	}
}

// set bools of return_index_until_new
void	set_bools_riun(bool *expandable,
		bool *in_quotes, bool to_expandable, bool to_quotes)
{
	if (expandable != NULL)
	{
		(*expandable) = to_expandable;
		(*in_quotes) = to_quotes;
	}
}

void	set_low_sup_s(bool *have_a_low_sup, int *lowers_superier_index,
		t_list_of_quotes *list_of_quotes, int *i)
{
	if ((*have_a_low_sup) == false)
		(*have_a_low_sup) = true;
	(*lowers_superier_index) = list_of_quotes->single_quotes[(*i)];
}
