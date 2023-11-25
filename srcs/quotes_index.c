/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_index.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 22:26:51 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 22:44:14 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_low_sup_s(bool *have_a_low_sup, int *lowers_superier_index,
			t_list_of_quotes *list_of_quotes, int *i);
void	set_bools_riun(bool *expandable,
			bool *in_quotes, bool to_expandable, bool to_quotes);

void	set_low_sup_d(bool *have_a_low_sup,
	int *lowers_superier_index, t_list_of_quotes *list_of_quotes, int *i)
{
	if ((*have_a_low_sup) == false)
		(*have_a_low_sup) = true;
	(*lowers_superier_index) = list_of_quotes->double_quotes[(*i)];
}

/*
	finds the next important index that
	 need to be slip
	
	INDEX_CUREN_CHAR is where I am
	function will return until when do I need to
		 copy, before the end or the start of a quotes
	and if EXPANDABLE is not NULL, it changes the
		 values of EXPANDABLE and IN_QUOTES
		 	depending on the return
t_list_of_quotes *list_of_quotes, int index_curent_char,
bool *expandable, bool *in_quotes
bool	have_a_low_sup;
int		lowers_superier_inportant_index;
*/
int	return_index_until_new(t_list_of_quotes *l_q,
		int i_cc, bool *exp, bool *i_q)
{
	int		i;
	bool	low;
	int		l_s_i;

	i = -1;
	low = false;
	l_s_i = -1;
	while (l_q->single_quotes[++i] != -1)
	{
		if (i % 2 == 0 && (low == false || l_q->single_quotes[i] < l_s_i))
			if (l_q->single_quotes[i] > i_cc)
				set_low_sup_s(&low, &l_s_i, l_q, &i);
		if (i % 2 == 0 && i_cc == l_q->single_quotes[i])
			return (set_bools_riun(exp, i_q, 0, 1), l_q->single_quotes[i + 1]);
	}
	i = -1;
	while (l_q->double_quotes[++i] != -1)
	{
		if (i % 2 == 0 && (low == false || l_q->double_quotes[i] < l_s_i))
			if (l_q->double_quotes[i] > i_cc)
				set_low_sup_d(&low, &l_s_i, l_q, &i);
		if (i % 2 == 0 && i_cc == l_q->double_quotes[i])
			return (set_bools_riun(exp, i_q, 1, 1), l_q->double_quotes[i + 1]);
	}
	return (set_bools_riun(exp, i_q, 1, 0), l_s_i);
}

/*
	check if double quotes and single quotes are next
	to each other or there is text in between.
	ex: 'test'"test" false
	ex: 'test'la"test" true
	
	END_QUOTES_SURCH is list of quotes where we are
	looking at the end
	START_QUOTES_SURCH is the list of quotes where we
	look at the start
	I is the current END_QUOTES_SURCH we are looking at
*/
bool	check_for_quotes_none_quotes(int *end_quotes_surch,
		int *start_quotes_surch, int i)
{
	int		j;
	bool	have_none_quotes;

	have_none_quotes = true;
	j = 0;
	while (start_quotes_surch[j] != -1)
	{
		if (end_quotes_surch[i] + 1 == start_quotes_surch[j])
			have_none_quotes = false;
		j++;
	}
	return (have_none_quotes);
}

/*
	Count how many quotes string are separated with '
	and " and if there is in between.

	Always assume there is something at the end,
	it does check, but check for front.
*/
int	count_how_many_quotes_content_separated(
	t_list_of_quotes *list_of_quotes, char *str)
{
	int		i;
	int		num_of_content_parts;

	i = -1;
	num_of_content_parts = 0;
	if (str[0] != '\'' && str[0] != '\"')
		num_of_content_parts++;
	while (list_of_quotes->double_quotes[++i] != -1)
	{
		if (list_of_quotes->double_quotes[i] % 2 == 1)
			if (check_for_quotes_none_quotes(list_of_quotes->double_quotes,
					list_of_quotes->single_quotes, i))
				num_of_content_parts++;
	}
	num_of_content_parts = num_of_content_parts + i / 2;
	i = -1;
	while (list_of_quotes->single_quotes[++i] != -1)
	{
		if (list_of_quotes->single_quotes[i] % 2 == 1)
			if (check_for_quotes_none_quotes(list_of_quotes->single_quotes,
					list_of_quotes->double_quotes, i))
				num_of_content_parts++;
	}
	num_of_content_parts = num_of_content_parts + i / 2;
	return (num_of_content_parts);
}

void	add_the_end_content(char *str,
	t_post_quotes ***content, int index_current_char, int *content_i)
{
	if (str[index_current_char + 1] == '\0')
	{
		printf("content_i = %i\n", (*content_i));
		free((*content)[(*content_i)]);
		return ;
	}
	(*content)[(*content_i)]->content = malloc(
			sizeof(char) * (ft_strlen(str) - index_current_char));
	if (!(*content)[(*content_i)]->content)
		exit(1);
	ft_strlcpy((*content)[(*content_i)]->content,
		&str[index_current_char + 1], ft_strlen(str) - index_current_char);
	(*content)[(*content_i)]->have_to_expand = true;
	(*content)[(*content_i)]->is_quotes = false;
	(*content_i)++;
}
