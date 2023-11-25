/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_acronyms.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 22:28:02 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 22:42:33 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_index_of_list_of_quotes(
			t_list_of_quotes **list_of_quotes, char *str);

/*
	find the possition of both type of quotes,
		and return it in the pointer of a struct T_LIST_OF_QUOTES.

	STR is the string we are taking the postion of the quotes, (user inpute)
*/
t_list_of_quotes	*count_and_locate_quotes(char *str)
{
	t_list_of_quotes	*list_of_quotes;
	int					num_single_quotes;
	int					num_double_quotes;

	count_and_locate_quotes_init(&num_single_quotes, &num_double_quotes, str);
	list_of_quotes = malloc(sizeof(t_list_of_quotes));
	if (!list_of_quotes)
		exit(1);
	list_of_quotes->double_quotes = (int *)malloc(sizeof(int)
			* (num_double_quotes + 1));
	list_of_quotes->single_quotes = (int *)malloc(sizeof(int)
			* (num_single_quotes + 1));
	if (!list_of_quotes->double_quotes || !list_of_quotes->single_quotes)
		exit (-1);
	set_index_of_list_of_quotes(&list_of_quotes, str);
	list_of_quotes->single_quotes[num_single_quotes] = -1;
	list_of_quotes->double_quotes[num_double_quotes] = -1;
	return (list_of_quotes);
}

// copy until double quotes
// og name rm_quotes_doll_cpy_d_quotes
char	*rm_q_d_cpy_d_q(char **str,
		t_list_of_quotes *list_of_quotes,
		int *current_double_quotes_index, int *i)
{
	char	*str_temp;

	str_temp = str_dup_until_index(&(*str)[(*i) + 1], list_of_quotes
			->double_quotes[(*current_double_quotes_index) + 1] - (*i) - 1);
	str_temp = handle_expand_doll(str_temp);
	(*i) = list_of_quotes->double_quotes[
		(*current_double_quotes_index) + 1] + 1;
	(*current_double_quotes_index)++;
	(*current_double_quotes_index)++;
	return (str_temp);
}

// copy until double quotes
// of name rm_quotes_doll_cpy_u_d_quotes
char	*rm_q_d_cpy_u_d_q(char **str,
		t_list_of_quotes *list_of_quotes,
		int *current_double_quotes_index, int *i)
{
	char	*str_temp;

	str_temp = str_dup_until_index(&(*str)[(*i)], list_of_quotes
			->double_quotes[(*current_double_quotes_index)] - (*i));
	str_temp = handle_expand_doll(str_temp);
	(*i) = list_of_quotes->double_quotes[(*current_double_quotes_index)];
	return (str_temp);
}

// copy single quotes
// of name rm_quotes_doll_cpy_s_quotes
char	*rm_q_d_cpy_s_q(char **str,
		t_list_of_quotes *list_of_quotes,
		int *current_single_quotes_index, int *i)
{
	char	*str_temp;

	str_temp = str_dup_until_index(&(*str)[(*i) + 1], list_of_quotes
			->single_quotes[(*current_single_quotes_index) + 1] - (*i) - 1);
	(*i) = list_of_quotes->single_quotes[
		(*current_single_quotes_index) + 1] + 1;
	(*current_single_quotes_index)++;
	(*current_single_quotes_index)++;
	return (str_temp);
}

// copy until single quotes
// og name rm_quotes_doll_cpy_u_s_quotes
char	*rm_q_d_cpy_u_s_q(char **str,
		t_list_of_quotes *list_of_quotes,
		int *current_single_quotes_index, int *i)
{
	char	*str_temp;

	str_temp = str_dup_until_index(&(*str)[(*i)], list_of_quotes
			->single_quotes[(*current_single_quotes_index)] - (*i));
	str_temp = handle_expand_doll(str_temp);
	(*i) = list_of_quotes->single_quotes[(*current_single_quotes_index)];
	return (str_temp);
}
