/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_lookup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 22:28:33 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 22:45:03 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		get_right_i_fake_quotes(int i,
			t_list_of_quotes **list_of_quotes, int j);
void	relocate_quotes(t_list_of_quotes **list_of_quotes,
			char **str, int **fake_double_quotes);
void	remove_fake_double_quotes(int
			**position_double_quotes, int *fake_double_quotes);

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
