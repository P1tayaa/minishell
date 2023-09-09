/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sboulain <sboulain@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 11:53:41 by omathot           #+#    #+#             */
/*   Updated: 2023/09/09 17:27:11 by sboulain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Handle ’ (single quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence.
Handle " (double quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence except for $ (dollar sign).
*/

int		*get_doll_position(char *str);
char	*handle_expand_doll(char **str);

int	get_number_of_real_double_quotes(int **position_double_quotes,  int	*fake_double_quotes)
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

void	remove_fake_double_quotes(int **position_double_quotes, int	*fake_double_quotes)
{
	int	i;
	int	j;
	int	num_of_real_quotes;
	int	*real_quotes;

	num_of_real_quotes = get_number_of_real_double_quotes(position_double_quotes, fake_double_quotes);
	real_quotes = (int *)malloc(sizeof(int) * (num_of_real_quotes + 1));
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
		{
			real_quotes[num_of_real_quotes] = (*position_double_quotes)[i];
			num_of_real_quotes++;
		}
		i++;
	}
	real_quotes[num_of_real_quotes] = -1;
	// printf("\nfirst real_quotes is %d\n", real_quotes[0]);
	free((*position_double_quotes));
	// free(fake_double_quotes);
	(*position_double_quotes) = real_quotes;
}

t_list_of_quotes *count_and_locate_quotes(char *str);
void	find_fake_quotes(t_list_of_quotes **list_of_quotes, char **str, int	**fake_double_quotes);

void	relocate_quotes(t_list_of_quotes **list_of_quotes, char **str, int	**fake_double_quotes)
{
	char *str_temp;

	if (fake_double_quotes != NULL)
		str_temp = readline("\n double quotes > ");
	else
		str_temp = readline("\n single quotes > ");
	str_temp = ft_strjoin(*str, str_temp);
	free(*str);
	*str = str_temp;
	rl_clear_history();
	add_history(str_temp);
	free((*list_of_quotes)->double_quotes);
	free((*list_of_quotes)->single_quotes);
	free(*list_of_quotes);
	*list_of_quotes = count_and_locate_quotes(*str);
	if (fake_double_quotes != NULL)
	{
		find_fake_quotes(list_of_quotes, str, fake_double_quotes);
		if ((*fake_double_quotes)[0] != -1)
			remove_fake_double_quotes(&(*list_of_quotes)->double_quotes, (*fake_double_quotes));
	}
}


void	find_fake_quotes(t_list_of_quotes **list_of_quotes, char **str, int	**fake_double_quotes)
{
	int	i;
	int	j;
	int	index_fake_double_quotes;
	int	val_of_curent_double_quote;

	i = 0;
	j = 0;
	index_fake_double_quotes = 0;
	while ((*list_of_quotes)->single_quotes[j] != -1)
	{
		val_of_curent_double_quote = (*list_of_quotes)->double_quotes[i];
		if ((*list_of_quotes)->single_quotes[j + 1] == -1)
		{
			relocate_quotes(list_of_quotes, str, NULL);
			continue ;
		}
		while (val_of_curent_double_quote != -1 && (*list_of_quotes)->single_quotes[j] > val_of_curent_double_quote)
		{
			i++;
			val_of_curent_double_quote = (*list_of_quotes)->double_quotes[i];
		}
		while (val_of_curent_double_quote != -1 && (*list_of_quotes)->single_quotes[j + 1] > val_of_curent_double_quote)
		{
			if ((*list_of_quotes)->double_quotes[i] != -1)
				(*fake_double_quotes)[index_fake_double_quotes] = val_of_curent_double_quote;
			index_fake_double_quotes++;
			i++;
			if ((*list_of_quotes)->double_quotes[i] == -1)
				break ;
		}
		j++;
		j++;
	}
	(*fake_double_quotes)[index_fake_double_quotes] = -1;
}

void	chekc_quotes_and_remove_fake_quotes(t_list_of_quotes **list_of_quotes, char **str)
{
	int	i;
	// int	j;
	// int	index_fake_double_quotes;
	// int	val_of_curent_double_quote;
	int	*fake_double_quotes;
	// char *str_temp;

	//! need to make fake_double_quotes malloc better, maybe dup sizeof double quotes or something
	fake_double_quotes = (int *)malloc(sizeof(int) * 20);
	find_fake_quotes(list_of_quotes, str, &fake_double_quotes);
	
	// printf("fake_double_quotes first is %d\n", fake_double_quotes[0]);
	if (fake_double_quotes[0] != -1)
		remove_fake_double_quotes(&(*list_of_quotes)->double_quotes, fake_double_quotes);
	// printf("real quotes 2 %d\n", (*list_of_quotes)->double_quotes[1]);

	i = 0;
	// j = 0;
	// index_fake_double_quotes = 0;
	while ((*list_of_quotes)->double_quotes[i] != -1)
	{
		// val_of_curent_double_quote = (*list_of_quotes)->double_quotes[i];
		// if (val_of_curent_single_quote > val_of_curent_double_quote)
		// {
			// printf("second_ double quotes %d", (*list_of_quotes)->double_quotes[i + 1]);
			if ((*list_of_quotes)->double_quotes[i + 1] == -1)
			{
				// str_temp = readline("\n double quotes > ");
				// str_temp = ft_strjoin(*str, str_temp);
				// free(*str);
				// *str = str_temp;
				// free((*list_of_quotes)->double_quotes);
				// free((*list_of_quotes)->single_quotes);
				// free(*list_of_quotes);
				// *list_of_quotes = count_and_locate_quotes(*str);
				// if (fake_double_quotes[0] != -1)
				// 	remove_fake_double_quotes(&(*list_of_quotes)->double_quotes, fake_double_quotes);
				relocate_quotes(list_of_quotes, str, &fake_double_quotes);
				continue ;
			}
			i++;
			i++;
		// }
	}
	free(fake_double_quotes);
}

// int	*count_double_quotes_quotes()
// {

// }

t_list_of_quotes *count_and_locate_quotes(char *str)
{
	t_list_of_quotes *list_of_quotes;
	int num_single_quotes;
	int num_double_quotes;
	int i;
	
	i = 0;
	num_single_quotes = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\'')
			num_single_quotes++;
		i++;
	}
	i = 0;
	num_double_quotes = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\"')
			num_double_quotes++;
		i++;
	}
	list_of_quotes = malloc(sizeof(t_list_of_quotes));
	// get position of all quotes
	
	list_of_quotes->double_quotes = (int *)malloc(sizeof(int) * (num_double_quotes + 1));
	list_of_quotes->single_quotes = (int *)malloc(sizeof(int) * (num_single_quotes + 1));
	if (!list_of_quotes->double_quotes || !list_of_quotes->single_quotes)
		exit (-1);
	i = 0;
	num_double_quotes = 0;
	num_single_quotes = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\'')
		{
			list_of_quotes->single_quotes[num_single_quotes] = i;
			num_single_quotes++;
		}
		if (str[i] == '\"')
		{
			list_of_quotes->double_quotes[num_double_quotes] = i;
			num_double_quotes++;
		}
		i++;
	}

	list_of_quotes->single_quotes[num_single_quotes] = -1;
	list_of_quotes->double_quotes[num_double_quotes] = -1;
	return (list_of_quotes);
}

// char	*handle_expand_doll(char **str);
char	*ft_strjoin_with_frees(char const *s1, char const *s2);

void	remove_quotes_and_expand_dollars(char **str, t_list_of_quotes *list_of_quotes)
{
	int i;
	int	current_single_quotes_index;
	int	current_double_quotes_index;
	char *final_str;
	char *str_temp;

	i = 0;
	current_double_quotes_index = 0;
	current_single_quotes_index = 0;
	final_str = NULL;
	if (list_of_quotes->double_quotes[current_double_quotes_index] != 0 && list_of_quotes->single_quotes[current_single_quotes_index] != 0)
	{
		if (list_of_quotes->double_quotes[current_double_quotes_index] < list_of_quotes->single_quotes[current_single_quotes_index])
		{
			i = list_of_quotes->double_quotes[current_double_quotes_index];
			final_str = str_dup_until_index((*str), list_of_quotes->double_quotes[current_double_quotes_index] - 1);
		}
		else
		{
			i = list_of_quotes->single_quotes[current_single_quotes_index];
			final_str = str_dup_until_index((*str), list_of_quotes->single_quotes[current_single_quotes_index] - 1);
		}
		final_str = handle_expand_doll(&final_str);
	}
	while ((*str)[i] != '\0')
	{
		if (i == list_of_quotes->double_quotes[current_double_quotes_index])
		{
			str_temp = str_dup_until_index(&(*str)[current_double_quotes_index + 1], list_of_quotes->double_quotes[current_double_quotes_index + 1]);
			str_temp = handle_expand_doll(&str_temp);
			final_str  = ft_strjoin_with_frees(final_str, str_temp);
			i = list_of_quotes->double_quotes[current_double_quotes_index + 1] + 1;
			current_double_quotes_index++;
			current_double_quotes_index++;
		}
		else if (i == list_of_quotes->single_quotes[current_single_quotes_index])
		{
			str_temp = str_dup_until_index(&(*str)[current_single_quotes_index + 1], list_of_quotes->single_quotes[current_single_quotes_index + 1]);
			// str_temp = handle_expand_doll(str_temp);
			final_str  = ft_strjoin_with_frees(final_str, str_temp);
			i = list_of_quotes->single_quotes[current_single_quotes_index + 1] + 1;
			current_single_quotes_index++;
			current_single_quotes_index++;
		}
		else if (list_of_quotes->double_quotes[current_double_quotes_index] != -1 && (list_of_quotes->double_quotes[current_double_quotes_index] < list_of_quotes->single_quotes[current_single_quotes_index] || list_of_quotes->single_quotes[current_single_quotes_index] == -1))
		{
			printf("i = %d, until %d, \n", i, list_of_quotes->double_quotes[current_double_quotes_index]);
			str_temp = str_dup_until_index(&(*str)[i], list_of_quotes->double_quotes[current_double_quotes_index] - i);
			str_temp = handle_expand_doll(&str_temp);
			final_str  = ft_strjoin_with_frees(final_str, str_temp);
			i = list_of_quotes->double_quotes[current_double_quotes_index];
		}
		else if (list_of_quotes->single_quotes[current_single_quotes_index] != -1 && (list_of_quotes->double_quotes[current_double_quotes_index] > list_of_quotes->single_quotes[current_single_quotes_index] || list_of_quotes->double_quotes[current_double_quotes_index] == -1))
		{
			str_temp = str_dup_until_index(&(*str)[i], list_of_quotes->single_quotes[current_single_quotes_index] - i);
			str_temp = handle_expand_doll(&str_temp);
			final_str  = ft_strjoin_with_frees(final_str, str_temp);
			i = list_of_quotes->single_quotes[current_single_quotes_index];
		}
		else if (list_of_quotes->double_quotes[current_double_quotes_index] == -1 && list_of_quotes->single_quotes[current_single_quotes_index] == -1)
		{
			str_temp = ft_strdup(&(*str)[i]);
			final_str  = ft_strjoin_with_frees(final_str, str_temp);
			free(*str);
			(*str) = final_str;
			puts(final_str);
			return ;
		}
		else
		{
			// not possible
			printf("problem at remove_quotes_and_expand_dollars, at char %d\n", i);
			exit(1);
		}
	}
	free(*str);
	(*str) = final_str;
	puts(final_str);
}

// here *str is &str[3] from main.
void	check_quotes(char *str)
{
	int	i;
	t_list_of_quotes *list_of_quotes;

	// count number of quotes
	list_of_quotes = count_and_locate_quotes(str);
	i = 0;
	while (list_of_quotes->double_quotes[i] != -1)
	{
		printf("double quotes %d at incex %d\n", i, list_of_quotes->double_quotes[i]);
		i++;
	}

	i = 0;
	while (list_of_quotes->single_quotes[i] != -1)
	{
		printf("single quotes %d at incex %d\n", i, list_of_quotes->single_quotes[i]);
		i++;
	}
	

	if (list_of_quotes->single_quotes[0] == -1)
	{
		//expand and check for doubles_quotes

	}
	else
	{
		chekc_quotes_and_remove_fake_quotes(&list_of_quotes, &str);
		// check all quotes are in pair from left to right
		// if not ask user to complete
	}

	puts("");
	i = 0;
	while (list_of_quotes->double_quotes[i] != -1)
	{
		printf("double quotes %d at incex %d\n", i, list_of_quotes->double_quotes[i]);
		i++;
	}

	i = 0;
	while (list_of_quotes->single_quotes[i] != -1)
	{
		printf("single quotes %d at incex %d\n", i, list_of_quotes->single_quotes[i]);
		i++;
	}


	remove_quotes_and_expand_dollars(&str, list_of_quotes);
	puts("");
	puts(str);

	// if (num_single_quotes % 2 == 1 || num_double_quotes % 2 == 1)
	// {
	// 	// * ask for user to finish him phrase
	// 	printf("missing %d single quotes, and missing %d double quotes", num_single_quotes % 2, num_double_quotes % 2);	 
	// 	exit (-1);
	// }

	// check if there are single quotes

	// if not just expand all

	// if there are, seperate what is single than expand all

	// if (position_single_quotes[0] < position_double_quotes[0])
	// // int	i;

	// // i = 0;
	// // while (str[i])
	// // {
	// 	// 39 is ascii value for single quotes BUT, \ is in front of it
	// 	if (str[0] == '\'' && str[ft_strlen(str) - 1] == '\'')
	// 		printf("%s\n", str);
	// 	// 34 is ascii value for double quotes,
	// 	// if (str[0] == '\"' && str[ft_strlen(str) - 1] == '\"')
	// 	else
	// 	{
	// 		char *str_temp;
	// 		str_temp = handle_expand_doll(str);
	// 		printf("%s\n", str_temp);
	// 		free(str_temp);
	// 		system("leaks -q minishell");
	// 	}
	// // }
}

char	**spit_text_args(char *str, int	*doll_pos);
char	*ft_strjoin_double_str(char **spit_text);
void	split_test_freeur(char **spit_text);

char	*handle_expand_doll(char **str)
{
	int	*doll_pos;
	char **spit_text;
	char *final_str;

	// printf("{%s}\n",str);
	doll_pos = get_doll_position((*str));
	if (doll_pos[0] == -1)
		{
			free(doll_pos);
			return (ft_strdup((*str)));
		}
	spit_text = spit_text_args((*str), doll_pos);
	final_str = ft_strjoin_double_str(spit_text);
	split_test_freeur(spit_text);
	free(doll_pos);
	puts(final_str);
	return (final_str);
}

char	*ft_strjoin_double_str(char **spit_text)
{
	int	i;
	int	j;
	int	size_str;
	char *str;

	size_str = 0;
	i = 0;
	while (spit_text[i] != NULL)
	{
		size_str = size_str + ft_strlen(spit_text[i]);
		i++;
	}
	str = malloc(sizeof(char) * (size_str + 1));
	if (!str)
		exit(EXIT_FAILURE);
	size_str = 0;
	i = 0;
	while (spit_text[i] != NULL)
	{
		j = 0;
		while (spit_text[i][j] != '\0')
		{
			str[size_str] = spit_text[i][j];
			size_str++;
			j++;
		}
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
		if (i % 2 == 0)
			free(spit_text[i]);
		i++;
	}
	free(spit_text);
	spit_text = NULL;
	
}

char *ft_strdup_intil_index_n(char *s1, int n)
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

bool	ft_isspace(unsigned char c);

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


char	**spit_text_args(char *str, int	*doll_pos)
{
	int		i;
	char	**string_split;
	int		num_doll;
	char	*temp_str;
	int		total_parts;

	num_doll = 0;
	while (doll_pos[num_doll] != -1)
		num_doll++;
	total_parts = (num_doll * 2) + 1;
	string_split = malloc(sizeof(char *) * (total_parts + 2));
	i = 0;
	num_doll = 0;
	while (i < (total_parts))
	{
		if (i % 2 == 0)
		{
			if (i == 0)
				string_split[i] = ft_strdup_intil_index_n(str, doll_pos[num_doll] - 1);
			else
			{
				if (doll_pos[num_doll] != -1)
					if (find_next_space(&str[doll_pos[num_doll - 1]] + 1) == -1 || find_next_space(&str[doll_pos[num_doll - 1]] + 1) > doll_pos[num_doll] - (doll_pos[num_doll - 1] + 1))
						string_split[i] = ft_strdup("\0");
					else
						string_split[i] = ft_strdup_intil_index_n(&str
							[doll_pos[num_doll - 1] + find_next_space(&str[doll_pos[num_doll - 1]])],
							doll_pos[num_doll]
							- (doll_pos[num_doll - 1] + find_next_space(&str[doll_pos[num_doll - 1]])) - 1);
				else
					if (find_next_space(&str[doll_pos[num_doll - 1]]) == -1)
						if (find_next_quote(&str[doll_pos[num_doll - 1]]) == -1)
							string_split[i] = ft_strdup("\0");
						else
							string_split[i] = ft_strdup("\"");
					else
						string_split[i] = ft_strdup_intil_index_n(&str
							[doll_pos[num_doll - 1] + find_next_space(&str[doll_pos[num_doll - 1]])],
							ft_strlen(&str
							[doll_pos[num_doll - 1] + find_next_space(&str[doll_pos[num_doll - 1]])]));
			}
		}
		else
		{
			if (find_next_space(&str[doll_pos[num_doll]]) == -1)
			{
				if (doll_pos[num_doll + 1] != -1)
					temp_str = ft_strdup_intil_index_n(&str[doll_pos[num_doll] + 1], doll_pos[num_doll + 1] - (doll_pos[num_doll] + 2));
				else
				{
					if (find_next_quote(&str[doll_pos[num_doll] + 1]) - 1 != -2)
						temp_str = ft_strdup_intil_index_n(&str[doll_pos[num_doll] + 1], find_next_quote(&str[doll_pos[num_doll] + 1]) - 1);
					else
					{
						printf("%d\n", doll_pos[num_doll] + 1);
						temp_str = ft_strdup(&str[doll_pos[num_doll] + 1]);
					}
				}
			}
			else if (doll_pos[num_doll + 1] != -1 && find_next_space(&str[doll_pos[num_doll]] + 1) > doll_pos[num_doll + 1] - (doll_pos[num_doll] + 1))
			{
				printf("\n %d, %d\n", doll_pos[num_doll + 1],  doll_pos[num_doll]);
				temp_str = ft_strdup_intil_index_n(&str[doll_pos[num_doll] + 1], doll_pos[num_doll + 1] - (doll_pos[num_doll] + 2));
			}
			else
				temp_str = ft_strdup_intil_index_n(&str[doll_pos[num_doll] + 1], find_next_space(&str[doll_pos[num_doll]] + 1) - 1);
			printf("\n%s\n", temp_str);
			string_split[i] = getenv(temp_str);
			// need to manage if faled
			free(temp_str);
			num_doll++;
		}
		i++;
	}
	string_split[i] = NULL;
	return (string_split);
}

int	*get_doll_position(char *str)
{
	int	i;
	int	num_doll;
	int	*doll_pos;

	i = 0;
	num_doll = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '$')
			num_doll++;
		i++;
	}
	doll_pos = malloc(sizeof(int) * (num_doll + 1));
	if (!doll_pos)
		exit(EXIT_FAILURE);
	i = 0;
	num_doll = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '$')
		{
			doll_pos[num_doll] = i;
			num_doll++;
		}
		i++;
	}
	doll_pos[num_doll] = -1;
	return (doll_pos);
}

// char **all;
// malloc (sizeof(char *) * parts);
// all[0] = strconais;
// all[1] = getenv(str);

// char *finish;
// finish(len2char())

// 1. look at first open - pattern matching     'asudgadagg"ausydgasdas"adhad';
// 2. getenv 



// we need a function to call when reading user input. spit_text_args doesnt do the job. 