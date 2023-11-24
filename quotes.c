/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 11:53:41 by omathot           #+#    #+#             */
/*   Updated: 2023/11/23 17:50:24 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Handle ’ (single quote) which should prevent
the shell from interpreting the metacharacters in
the quoted sequence.
Handle " (double quote) which should prevent
the shell from interpreting the metacharacters in
the quoted sequence except for $ (dollar sign).
*/

int					*get_doll_position(char *str);
char				*handle_expand_doll(char *str);
t_list_of_quotes	*count_and_locate_quotes(char *str);
void				find_fake_quotes(t_list_of_quotes **list_of_quotes,
						char **str, int	**fake_double_quotes);
char				*sjoin_fr(char const *s1, char const *s2);

int	get_number_of_real_double_quotes(int
		**position_double_quotes, int *fake_double_quotes)
{
	int	i;
	int	j;
	int	num_of_real_quotes;

	i = 0;
	num_of_real_quotes = 0;
	while ((*position_double_quotes)[i] != -1)
	{
		j = 0;
		while (fake_double_quotes[j] != -1)
		{
			if ((*position_double_quotes)[i] == fake_double_quotes[j])
				break ;
			j++;
		}
		if ((*position_double_quotes)[i] != fake_double_quotes[j])
			num_of_real_quotes++;
		i++;
	}
	return (num_of_real_quotes);
}

void	remove_fake_double_quotes_p2(int
		**position_double_quotes, int *fake_double_quotes,
		int *num_of_real_quotes, int **real_quotes)
{
	int	j;
	int	i;

	i = 0;
	while ((*position_double_quotes)[i] != -1)
	{
		j = 0;
		while (fake_double_quotes[j] != -1)
		{
			if ((*position_double_quotes)[i] == fake_double_quotes[j])
				break ;
			j++;
		}
		if ((*position_double_quotes)[i] != fake_double_quotes[j])
		{
			(*real_quotes)[(*num_of_real_quotes)]
				= (*position_double_quotes)[i];
			(*num_of_real_quotes)++;
		}
		i++;
	}
	(*real_quotes)[(*num_of_real_quotes)] = -1;
}

void	remove_fake_double_quotes(int
		**position_double_quotes, int *fake_double_quotes)
{
	int	num_of_real_quotes;
	int	*real_quotes;

	num_of_real_quotes = get_number_of_real_double_quotes(
			position_double_quotes, fake_double_quotes);
	printf("num_of_real_quotes == %i\n", num_of_real_quotes);
	real_quotes = (int *)malloc(sizeof(int) * (num_of_real_quotes + 5));
	if (!real_quotes)
		exit(1);
	remove_fake_double_quotes_p2(position_double_quotes,
		fake_double_quotes, &num_of_real_quotes, &real_quotes);
	free((*position_double_quotes));
	(*position_double_quotes) = real_quotes;
}

/*
	get more user input and adds it to the str (userinput)
	then refind the quotes and relocate the fakequotes (recursive loop)
*/
void	relocate_quotes(t_list_of_quotes **list_of_quotes,
		char **str, int **fake_double_quotes)
{
	char	*str_temp;

	if (fake_double_quotes != NULL)
		str_temp = readline("\n double quotes > ");
	else
		str_temp = readline("\n single quotes > ");
	*str = sjoin_fr(*str, ft_strjoin("\n", str_temp));
	free((*list_of_quotes)->double_quotes);
	free((*list_of_quotes)->single_quotes);
	free(*list_of_quotes);
	*list_of_quotes = count_and_locate_quotes(*str);
	if (fake_double_quotes != NULL)
	{
		find_fake_quotes(list_of_quotes, str, fake_double_quotes);
		if ((*fake_double_quotes)[0] != -1)
			remove_fake_double_quotes(
				&(*list_of_quotes)->double_quotes, (*fake_double_quotes));
	}
}

// void	assign_fake_quotes(t_list_of_quotes **list_of_quotes,
//		int *val_of_curent_double_quote, int *fake_double_quotes_val, int *i)
// {
// 	puts("in bs function");
// 	if ((*list_of_quotes)->double_quotes[(*i)] != -1)
// 	{
// 		puts("double quotes found in if");
// 		printf("curent (*val_of_curent_double_quote)
//		= %i\n",(*val_of_curent_double_quote));
// 		(*fake_double_quotes_val) = (*val_of_curent_double_quote);
// 	}
// 	//(*index_fake_double_quotes) = (*index_fake_double_quotes) + 1;
// 	(*i) = (*i) + 1;
// 	(*val_of_curent_double_quote) = (*list_of_quotes)->double_quotes[(*i)];
// }

int	get_right_i_fake_quotes(int i, t_list_of_quotes **list_of_quotes, int j)
{
	while ((*list_of_quotes)->double_quotes[i] != -1
		&& (*list_of_quotes)->single_quotes[j]
		> (*list_of_quotes)->double_quotes[i])
		i++;
	return (i);
}

// ind_f_d_q = index_fake_double_quotes
// f_d_q = fake_double_quotes
void	find_fake_quotes(t_list_of_quotes **list_of_quotes,
		char **str, int	**f_d_q)
{
	int	i;
	int	j;
	int	ind_f_d_q;

	i = 0;
	j = 0;
	ind_f_d_q = 0;
	while ((*list_of_quotes)->single_quotes[j] != -1)
	{
		if ((*list_of_quotes)->single_quotes[j + 1] != -1)
		{
			i = get_right_i_fake_quotes(i, list_of_quotes, j);
			while ((*list_of_quotes)->double_quotes[i] != -1 && (*list_of_quotes
				)->single_quotes[j + 1] > (*list_of_quotes)->double_quotes[i])
			{
				if ((*list_of_quotes)->double_quotes[i] != -1)
					(*f_d_q)[ind_f_d_q++] = (*list_of_quotes)->double_quotes[i];
				i++;
			}
			j = j + 2;
		}
		else
			relocate_quotes(list_of_quotes, str, NULL);
	}
	(*f_d_q)[ind_f_d_q] = -1;
}

/*
	Find double quotes are surounded by single
		quotes and remove them from consideration.
	So if they find it they will change the values of list of quotes.
*/
void	chekc_quotes_and_remove_fake_quotes(
		t_list_of_quotes **list_of_quotes, char **str)
{
	int	i;
	int	*fake_double_quotes;
	int	num_of_double_quotes;

	num_of_double_quotes = 0;
	while ((*list_of_quotes)->double_quotes[num_of_double_quotes] != -1)
	{
		num_of_double_quotes++;
	}
	fake_double_quotes = malloc(sizeof(int) * (num_of_double_quotes + 1));
	if (!fake_double_quotes)
		exit(1);
	find_fake_quotes(list_of_quotes, str, &fake_double_quotes);
	if (fake_double_quotes[0] != -1)
		remove_fake_double_quotes(&(*list_of_quotes)
			->double_quotes, fake_double_quotes);
	i = 0;
	while ((*list_of_quotes)->double_quotes[i] != -1)
	{
		if ((*list_of_quotes)->double_quotes[i + 1] == -1)
			relocate_quotes(list_of_quotes, str, &fake_double_quotes);
		else
			i = i + 2;
	}
	free(fake_double_quotes);
}

// count how many " and ' there are
void	count_and_locate_quotes_init(
		int *num_single_quotes, int *num_double_quotes, char *str)
{
	int	i;

	i = 0;
	(*num_single_quotes) = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\'')
			(*num_single_quotes)++;
		i++;
	}
	i = 0;
	(*num_double_quotes) = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\"')
			(*num_double_quotes)++;
		i++;
	}
}

// put the index location in each list of quotes
void	set_index_of_list_of_quotes(
		t_list_of_quotes **list_of_quotes, char *str)
{
	int	i;
	int	num_single_quotes;
	int	num_double_quotes;

	i = 0;
	num_double_quotes = 0;
	num_single_quotes = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\'')
		{
			(*list_of_quotes)->single_quotes[num_single_quotes] = i;
			num_single_quotes++;
		}
		if (str[i] == '\"')
		{
			(*list_of_quotes)->double_quotes[num_double_quotes] = i;
			num_double_quotes++;
		}
		i++;
	}
}

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

void	fill_content_nq(char *str,
		t_list_of_quotes *list_of_quotes,
		int index_current_char, t_post_quotes ***content)
{
	int	*content_i;

	content_i = list_of_quotes->i_temp;
	(*content)[(*content_i)]->content = malloc(sizeof(char)
			* (return_index_until_new(list_of_quotes,
					index_current_char, NULL, NULL) - index_current_char + 1));
	if (!(*content)[(*content_i)]->content)
		exit(1);
	ft_strlcpy((*content)[(*content_i)]->content, &str[index_current_char],
		return_index_until_new(list_of_quotes, index_current_char,
			&(*content)[(*content_i)]->have_to_expand, &(*content)[
			(*content_i)]->is_quotes) - index_current_char + 1);
}

void	fill_content_q(char *str,
		t_list_of_quotes *list_of_quotes,
			int index_current_char, t_post_quotes ***content)
{
	int	*content_i;

	content_i = list_of_quotes->i_temp;
	(*content)[(*content_i)]->content = malloc(sizeof(char)
			* (return_index_until_new(list_of_quotes,
					index_current_char, NULL, NULL) - index_current_char));
	if (!(*content)[(*content_i)]->content)
		exit(1);
	ft_strlcpy((*content)[(*content_i)]->content, &str[index_current_char + 1],
		return_index_until_new(list_of_quotes, index_current_char, &(*content)[
			(*content_i)]->have_to_expand, &(*content)[(*content_i)]
			->is_quotes) - index_current_char);
}

t_post_quotes	**malloc_big_content(char *str,
		t_list_of_quotes **list_of_quotes)
{
	t_post_quotes	**content;

	content = (t_post_quotes **)malloc(sizeof(t_post_quotes)
			* (count_how_many_quotes_content_separated((*list_of_quotes),
					str) + 1));
	if (!content)
		exit(1);
	(*(*list_of_quotes)->i_temp) = -1;
	return (content);
}

t_post_quotes	*malloc_small_content(void)
{
	t_post_quotes	*single_content;

	single_content = (t_post_quotes *)malloc(sizeof(t_post_quotes));
	if (!single_content)
		exit(1);
	return (single_content);
}

t_post_quotes	**make_post_quotes_content(char *str,
		t_list_of_quotes *list_of_quotes)
{
	static int		content_i = -1;
	t_post_quotes	**content;
	int				j;

	list_of_quotes->i_temp = &content_i;
	j = 0;
	content = malloc_big_content(str, &list_of_quotes);
	while (str[j] != '\0' && ++content_i > -1)
	{
		if (return_index_until_new(list_of_quotes, j, NULL, NULL) == j + 1)
			j++;
		content[content_i] = malloc_small_content();
		if (return_index_until_new(list_of_quotes, j, NULL, NULL) == -1)
		{
			add_the_end_content(str, &content, j, &content_i);
			break ;
		}
		if (str[0] != '\'' && str[0] != '\"' && j == 0)
			fill_content_nq(str, list_of_quotes, j, &content);
		else
			fill_content_q(str, list_of_quotes, j, &content);
		j = return_index_until_new(list_of_quotes, j, NULL, NULL);
	}
	content[content_i] = NULL;
	return (content);
}

/*
	This the main of the quotes.
	So if there is not quotes in STR_OG:
	It expand_doll and and doesn't do anything
		to content, leaving it NULL;
	If there is a quotes:
	It will find them than seperate the content
		insinde and out side of quotes and puts it into content.

	STR_OF is a pointer to the string, the userinput
	CONTENT is a pointer to the array of pointer
		of T_POST_QUOTES, (fake return)
*/
void	check_quotes(char **str_og, t_post_quotes ***content)
{
	t_list_of_quotes	*list_of_quotes;
	char				*str;

	str = (*str_og);
	list_of_quotes = count_and_locate_quotes(str);
	if (list_of_quotes->single_quotes[0] == -1
		&& list_of_quotes->double_quotes[0] == -1)
	{
		add_history(str);
		str = handle_expand_doll((*str_og));
		free((*str_og));
		(*str_og) = str;
	}
	else
	{
		chekc_quotes_and_remove_fake_quotes(&list_of_quotes, &str);
		(*str_og) = str;
		add_history(str);
		(*content) = make_post_quotes_content(str, list_of_quotes);
	}
	free(list_of_quotes->double_quotes);
	free(list_of_quotes->single_quotes);
	free(list_of_quotes);
}

char				**spit_text_args(char *str, int	*doll_pos);
char				*ft_strjoin_double_str(char **spit_text);
void				split_test_freeur(char **spit_text);
char				*get_env_of_valus_str(char *str);

char	*handle_expand_doll(char *str)
{
	int		*doll_pos;
	char	**spit_text;
	char	*final_str;

	doll_pos = get_doll_position(str);
	if (doll_pos[0] == -1)
	{
		free(doll_pos);
		return (ft_strdup(str));
	}
	spit_text = spit_text_args(str, doll_pos);
	printf("spit_text[0] in handle = (%s)\n", spit_text[0]);
	printf("spit_text[1] in handle = (%s)\n", spit_text[1]);
	printf("spit_text[2] in handle = (%s)\n", spit_text[2]);
	final_str = ft_strjoin_double_str(spit_text);
	split_test_freeur(spit_text);
	free(doll_pos);
	return (final_str);
}

void	ft_strjoin_double_str_copy_to(char **spit_text, char **str)
{
	int	i;
	int	j;
	int	size_str;

	i = 0;
	size_str = 0;
	while (spit_text[i] != NULL)
	{
		j = 0;
		while (spit_text[i][j] != '\0')
		{
			(*str)[size_str] = spit_text[i][j];
			size_str++;
			j++;
		}
		i++;
	}
}

char	*ft_strjoin_double_str(char **spit_text)
{
	int		i;
	int		j;
	int		size_str;
	char	*str;

	size_str = 0;
	i = 0;
	while (spit_text[i] != NULL)
		size_str = size_str + ft_strlen(spit_text[i++]);
	str = malloc(sizeof(char) * (size_str + 1));
	if (!str)
		exit(EXIT_FAILURE);
	size_str = 0;
	i = 0;
	while (spit_text[i] != NULL)
	{
		j = 0;
		while (spit_text[i][j] != '\0')
			str[size_str++] = spit_text[i][j++];
		i++;
	}
	str[size_str] = '\0';
	return (str);
}

void	split_test_freeur(char **spit_text)
{
	int	i;

	i = 0;
	while (spit_text[i] != NULL)
	{
		free(spit_text[i]);
		i++;
	}
	free(spit_text);
	spit_text = NULL;
}

char	*ft_strdup_intil_index_n(char *s1, int n)
{
	int		i;
	char	*c;

	i = 0;
	c = malloc(sizeof(char) * (n + 2));
	if (!c)
		return (NULL);
	while (i <= n)
	{
		c[i] = s1[i];
		i++;
	}
	c[i] = '\0';
	return (c);
}

bool				ft_isspace(unsigned char c);

int	find_next_space(char *str)
{
	int	i;

	i = 0;
	while (!ft_isspace(str[i]))
	{
		if (str[i] == '\0')
			return (-1);
		i++;
	}
	return (i);
}

int	find_next_quote(char *str)
{
	int	i;

	i = 0;
	while (!(str[i] == '\"'))
	{
		if (str[i] == '\0')
			return (-1);
		i++;
	}
	return (i);
}

/*
char	*spit_text_args_even(char *str, int	*doll_pos,
		int num_doll, char *string_split)

*/
char	*spit_text_args_even(char *str, int	*d_p,
		int n_d, char **s_s)
{
	if (d_p[n_d] != -1)
	{
		if (find_next_space(&str[d_p[n_d - 1]] + 1) == -1 || find_next_space(
				&str[d_p[n_d - 1]] + 1) > d_p[n_d] - (d_p[n_d - 1] + 1))
			(*s_s) = ft_strdup("\0");
		else
			(*s_s) = ft_strdup_intil_index_n(&str[d_p[n_d - 1]
					+ find_next_space(&str[d_p[n_d - 1]])], d_p[n_d]
					- (d_p[n_d - 1] + find_next_space(&str[d_p[n_d - 1]])) - 1);
	}
	else
	{
		if (find_next_space(&str[d_p[n_d - 1]]) == -1)
		{
			if (find_next_quote(&str[d_p[n_d - 1]]) == -1)
				(*s_s) = ft_strdup("\0");
			else
				(*s_s) = ft_strdup("\"");
		}
		else
			(*s_s) = ft_strdup_intil_index_n(&str[d_p[n_d - 1]
					+ find_next_space(&str[d_p[n_d - 1]])], ft_strlen(&str[
						d_p[n_d - 1] + find_next_space(&str[d_p[n_d - 1]])]));
	}
	return ((*s_s));
}

void	spit_text_args_odd(char *str, int *doll_pos,
		int num_doll, char	**temp_str)
{
	if (find_next_space(&str[doll_pos[num_doll]]) == -1)
	{
		if (doll_pos[num_doll + 1] != -1)
			(*temp_str) = ft_strdup_intil_index_n(&str[doll_pos[
					num_doll] + 1], doll_pos[num_doll + 1]
					- (doll_pos[num_doll] + 2));
		else
		{
			if (find_next_quote(&str[doll_pos[num_doll] + 1]) - 1 != -2)
				(*temp_str) = ft_strdup_intil_index_n(&str[doll_pos
					[num_doll] + 1], find_next_quote(
							&str[doll_pos[num_doll] + 1]) - 1);
			else
				(*temp_str) = ft_strdup(&str[doll_pos[num_doll] + 1]);
		}
	}
	else if (doll_pos[num_doll + 1] != -1 && find_next_space(&str[
				doll_pos[num_doll]] + 1) > doll_pos[
			num_doll + 1] - (doll_pos[num_doll] + 1))
		(*temp_str) = ft_strdup_intil_index_n(&str[doll_pos
			[num_doll] + 1], doll_pos[num_doll + 1] - (doll_pos[num_doll] + 2));
	else
		(*temp_str) = ft_strdup_intil_index_n(&str[doll_pos
			[num_doll] + 1], find_next_space(&str[doll_pos[num_doll]] + 1) - 1);
}

void	spit_text_args_odd_p2(char	**string_split, char **temp_str, int i)
{
	char	*temp;

	temp = get_env_of_valus_str((*temp_str));
	if (temp != NULL)
		string_split[i] = temp;
	else if ((*temp_str)[0] == '?' && (*temp_str)[1] == '\0')
	{
		string_split[i] = ft_strdup("$!");
	}
	else
	{
		string_split[i] = ft_strdup("");
	}
	free((*temp_str));
}

void	spit_text_args_init(int *num_doll, int *doll_pos,
		int *total_parts, char ***string_split)
{
	(*num_doll) = 0;
	while (doll_pos[(*num_doll)] != -1)
	{
		printf("doll poss is (%d)\n", doll_pos[(*num_doll)]);
		(*num_doll)++;
	}
	(*total_parts) = ((*num_doll) * 2) + 1;
	(*string_split) = malloc(sizeof(char *)
			* ((*total_parts) + 2));
	if (!(*string_split))
		exit(1);
}

void	terminate_string_split(char	***s_s, int i)
{
	(*s_s)[i] = NULL;
}

/*
	splite the STR into str and the dollars,
		and also replace the dollar with their value.
	Or for $? make it $! to be catch later. 
	And return the array of str.
	
	string_split = s_s
*/
char	**spit_text_args(char *str, int	*doll_pos)
{
	int		i;
	char	**s_s;
	int		num_doll;
	char	*temp_str;
	int		total_parts;

	spit_text_args_init(&num_doll, doll_pos, &total_parts, &s_s);
	i = -1;
	num_doll = 0;
	while (++i < (total_parts))
	{
		if (i % 2 == 0)
		{
			if (i == 0)
				s_s[i] = ft_strdup_intil_index_n(str, doll_pos[num_doll] - 1);
			else
				spit_text_args_even(str, doll_pos, num_doll, &(s_s[i]));
		}
		else
		{
			spit_text_args_odd(str, doll_pos, num_doll++, &temp_str);
			spit_text_args_odd_p2(s_s, &temp_str, i);
		}
	}
	return (terminate_string_split(&s_s, i), s_s);
}

int	*get_doll_position(char *str)
{
	int	i;
	int	num_doll;
	int	*doll_pos;

	i = 0;
	num_doll = 0;
	while (str[i] != '\0')
		if (str[i++] == '$')
			if (str[i] != '\0')
				if (str[i] != '=')
					num_doll++;
	doll_pos = malloc(sizeof(int) * (num_doll + 1));
	if (!doll_pos)
		exit(EXIT_FAILURE);
	i = -1;
	num_doll = 0;
	while (str[++i] != '\0')
	{
		if (str[i] == '$')
			if (str[i + 1] != '\0')
				if (str[i + 1] != '=')
					doll_pos[num_doll++] = i;
	}
	doll_pos[num_doll] = -1;
	return (doll_pos);
}
