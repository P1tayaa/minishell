/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 11:53:41 by omathot           #+#    #+#             */
/*   Updated: 2023/11/25 22:28:47 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
char				*sjoin_fr(char *s1, char *s2);

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
