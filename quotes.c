/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 11:53:41 by omathot           #+#    #+#             */
/*   Updated: 2023/10/17 15:28:22 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Handle ’ (single quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence.
Handle " (double quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence except for $ (dollar sign).
*/

int		*get_doll_position(char *str);
char	*handle_expand_doll(char *str);

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

void	remove_fake_double_quotes_p2(int **position_double_quotes, int	*fake_double_quotes, int *num_of_real_quotes, int	**real_quotes)
{
	int j;
	int i;

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
			(*real_quotes)[(*num_of_real_quotes)] = (*position_double_quotes)[i];
			(*num_of_real_quotes)++;
		}
		i++;
	}
	(*real_quotes)[(*num_of_real_quotes)] = -1;
}

void	remove_fake_double_quotes(int **position_double_quotes, int	*fake_double_quotes)
{
	// int	i;
	// int	j;
	int	num_of_real_quotes;
	int	*real_quotes;

	num_of_real_quotes = get_number_of_real_double_quotes(position_double_quotes, fake_double_quotes);
	real_quotes = (int *)malloc(sizeof(int) * (num_of_real_quotes + 1));
	if (!real_quotes)
		exit(1);
	remove_fake_double_quotes_p2(position_double_quotes, fake_double_quotes, &num_of_real_quotes, &real_quotes);
	free((*position_double_quotes));
	(*position_double_quotes) = real_quotes;
}

t_list_of_quotes *count_and_locate_quotes(char *str);
void	find_fake_quotes(t_list_of_quotes **list_of_quotes, char **str, int	**fake_double_quotes);
char	*ft_strjoin_with_frees(char const *s1, char const *s2);


/*
	get more user input and adds it to the str (userinput)
	then refind the quotes and relocate the fakequotes (recursive loop)
*/
void	relocate_quotes(t_list_of_quotes **list_of_quotes, char **str, int	**fake_double_quotes)
{
	char *str_temp;

	if (fake_double_quotes != NULL)
		str_temp = readline("\n double quotes > ");
	else
		str_temp = readline("\n single quotes > ");
	*str = ft_strjoin_with_frees(*str, ft_strjoin("\n", str_temp));
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
			val_of_curent_double_quote = (*list_of_quotes)->double_quotes[i];
			if ((*list_of_quotes)->double_quotes[i] == -1)
				break ;
		}
		j = j + 2;
	}
	(*fake_double_quotes)[index_fake_double_quotes] = -1;
}

/*
	Find double quotes are surounded by single quotes and remove them from consideration.
	So if they find it they will change the values of list of quotes.

	
*/
void	chekc_quotes_and_remove_fake_quotes(t_list_of_quotes **list_of_quotes, char **str)
{
	int	i;
	int	*fake_double_quotes;
	int	num_of_double_quotes;
	
	num_of_double_quotes = 0;
	while ((*list_of_quotes)->double_quotes[num_of_double_quotes] != -1)
	{
		num_of_double_quotes++;
	}
	fake_double_quotes = (int *)malloc(sizeof(int) * (num_of_double_quotes + 1));
	if (!fake_double_quotes)
		exit(1);
	find_fake_quotes(list_of_quotes, str, &fake_double_quotes);
	if (fake_double_quotes[0] != -1)
		remove_fake_double_quotes(&(*list_of_quotes)->double_quotes, fake_double_quotes);
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
void	count_and_locate_quotes_init(int *num_single_quotes, int *num_double_quotes, char *str)
{
	int i;

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
void	set_index_of_list_of_quotes(t_list_of_quotes **list_of_quotes, char *str)
{
	int i;
	int num_single_quotes;
	int num_double_quotes;
	
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
	find the possition of both type of quotes, and return it in the pointer of a struct T_LIST_OF_QUOTES.

	STR is the string we are taking the postion of the quotes, (user inpute)
*/
t_list_of_quotes *count_and_locate_quotes(char *str)
{
	t_list_of_quotes *list_of_quotes;
	int num_single_quotes;
	int num_double_quotes;

	count_and_locate_quotes_init(&num_single_quotes, &num_double_quotes, str);
	// Todo: make this a separate funciton
	// malloc the list of position of both type of quotes
	list_of_quotes = malloc(sizeof(t_list_of_quotes));
	if (!list_of_quotes)
		exit(1);
	list_of_quotes->double_quotes = (int *)malloc(sizeof(int) * (num_double_quotes + 1));
	list_of_quotes->single_quotes = (int *)malloc(sizeof(int) * (num_single_quotes + 1));
	if (!list_of_quotes->double_quotes || !list_of_quotes->single_quotes)
		exit (-1);
	set_index_of_list_of_quotes(&list_of_quotes, str);
	list_of_quotes->single_quotes[num_single_quotes] = -1;
	list_of_quotes->double_quotes[num_double_quotes] = -1;
	return (list_of_quotes);
}

// char	*handle_expand_doll(char **str);


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
		final_str = handle_expand_doll(final_str);
	}
	while ((*str)[i] != '\0')
	{
		if (i == list_of_quotes->double_quotes[current_double_quotes_index])
		{
			// copy double quotes
			// printf("i = %d, until %d, curent quotes %d \n", i, list_of_quotes->double_quotes[current_double_quotes_index + 1], current_double_quotes_index);

			str_temp = str_dup_until_index(&(*str)[i + 1], list_of_quotes->double_quotes[current_double_quotes_index + 1] - i - 1);
			// puts(str_temp);
			str_temp = handle_expand_doll(str_temp);
			final_str  = ft_strjoin_with_frees(final_str, str_temp);
			i = list_of_quotes->double_quotes[current_double_quotes_index + 1] + 1;
			current_double_quotes_index++;
			current_double_quotes_index++;
		}
		else if (i == list_of_quotes->single_quotes[current_single_quotes_index])
		{
			// copy single quotes
			str_temp = str_dup_until_index(&(*str)[i + 1], list_of_quotes->single_quotes[current_single_quotes_index + 1] - i - 1);
			// str_temp = handle_expand_doll(str_temp);
			final_str  = ft_strjoin_with_frees(final_str, str_temp);
			i = list_of_quotes->single_quotes[current_single_quotes_index + 1] + 1;
			current_single_quotes_index++;
			current_single_quotes_index++;
		}
		else if (list_of_quotes->double_quotes[current_double_quotes_index] != -1 && (list_of_quotes->double_quotes[current_double_quotes_index] < list_of_quotes->single_quotes[current_single_quotes_index] || list_of_quotes->single_quotes[current_single_quotes_index] == -1))
		{
			// copy until double quotes
			// printf("i = %d, until %d, \n", i, list_of_quotes->double_quotes[current_double_quotes_index]);
			str_temp = str_dup_until_index(&(*str)[i], list_of_quotes->double_quotes[current_double_quotes_index] - i);
			str_temp = handle_expand_doll(str_temp);
			final_str  = ft_strjoin_with_frees(final_str, str_temp);
			i = list_of_quotes->double_quotes[current_double_quotes_index];
		}
		else if (list_of_quotes->single_quotes[current_single_quotes_index] != -1 && (list_of_quotes->double_quotes[current_double_quotes_index] > list_of_quotes->single_quotes[current_single_quotes_index] || list_of_quotes->double_quotes[current_double_quotes_index] == -1))
		{
			// copy until single quotes
			str_temp = str_dup_until_index(&(*str)[i], list_of_quotes->single_quotes[current_single_quotes_index] - i);
			str_temp = handle_expand_doll(str_temp);
			final_str  = ft_strjoin_with_frees(final_str, str_temp);
			i = list_of_quotes->single_quotes[current_single_quotes_index];
		}
		else if (list_of_quotes->double_quotes[current_double_quotes_index] == -1 && list_of_quotes->single_quotes[current_single_quotes_index] == -1)
		{
			//only regular text left
			str_temp = ft_strdup(&(*str)[i]);
			final_str  = ft_strjoin_with_frees(final_str, str_temp);
			free(*str);
			(*str) = final_str;
			// puts(final_str);
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
	// puts(final_str);
}

// prints list of coordinates of quotes
void	print_coordines_of_all_quotes(t_list_of_quotes *list_of_quotes)
{
	int i;

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
}
/*
	finds the next important index that need to be slip
	
	INDEX_CUREN_CHAR is where I am
	function will return until when do I need to copy, before the end or the start of a quotes
	and if EXPANDABLE is not NULL, it changes the values of EXPANDABLE and IN_QUOTES depending on the return
*/
int	return_index_until_new(t_list_of_quotes *list_of_quotes, int index_curent_char, bool *expandable, bool *in_quotes)
{
	int		i;
	bool	have_a_low_sup;
	int		lowers_superier_inportant_index;

	i = 0;
	have_a_low_sup = false;
	lowers_superier_inportant_index = -1;
	// puts("test2");
	while (list_of_quotes->single_quotes[i] != -1)
	{
		if (i % 2 == 0 && (have_a_low_sup == false || list_of_quotes->single_quotes[i] < lowers_superier_inportant_index))
		{
			if (list_of_quotes->single_quotes[i] > index_curent_char)
			{
				if (have_a_low_sup == false)
					have_a_low_sup = true;
				lowers_superier_inportant_index = list_of_quotes->single_quotes[i];
			}
		}
		if (i % 2 == 0 && index_curent_char == list_of_quotes->single_quotes[i])
		{
			if (expandable != NULL)
			{
				(*expandable) = false;
				(*in_quotes) = true;
			}
			return (list_of_quotes->single_quotes[i + 1]);
		}
		i++;
	}
	i = 0;
	while (list_of_quotes->double_quotes[i] != -1)
	{
		if (i % 2 == 0 && (have_a_low_sup == false || list_of_quotes->double_quotes[i] < lowers_superier_inportant_index))
		{
			if (list_of_quotes->double_quotes[i] > index_curent_char)
			{
				if (have_a_low_sup == false)
					have_a_low_sup = true;
				lowers_superier_inportant_index = list_of_quotes->double_quotes[i];
			}
		}
		if (i % 2 == 0 && index_curent_char == list_of_quotes->double_quotes[i])
		{
			if (expandable != NULL)
			{
				(*expandable) = true;
				(*in_quotes) = true;
			}
			return (list_of_quotes->double_quotes[i + 1]);
		}
		i++;
	}
	if (expandable != NULL)
	{
		(*expandable) = true;
		(*in_quotes) = false;
	}
	return (lowers_superier_inportant_index);
}

/*
	check if double quotes and single quotes are next to each other or there is text in between.
	ex: 'test'"test" false
	ex: 'test'la"test" true
	
	END_QUOTES_SURCH is list of quotes where we are looking at the end
	START_QUOTES_SURCH is the list of quotes where we look at the start
	I is the current END_QUOTES_SURCH we are looking at
*/
bool check_for_quotes_none_quotes(int *end_quotes_surch, int *start_quotes_surch, int i)
{
	int		j;
	bool	have_none_quotes;

	have_none_quotes = true;
	j = 0;
	while (start_quotes_surch[j] != -1)
	{
		// printf("end is %d, start is %d\n", end_quotes_surch[i], start_quotes_surch[j]);
		if (end_quotes_surch[i] + 1 == start_quotes_surch[j])
			have_none_quotes = false;
		j++;
	}
	return (have_none_quotes);
}

/*
	Count how many quotes string are separated with ' and " and if there is in between.

	Always assume there is something at the end, it does check, but check for front.
*/
int	count_how_many_quotes_content_separated(t_list_of_quotes *list_of_quotes, char *str)
{
	int		i;
	int		num_of_content_parts;
	
	i = 0;
	num_of_content_parts = 0;
	if (str[0] != '\'' && str[0] != '\"')
		num_of_content_parts++;
	while (list_of_quotes->double_quotes[i] != -1)
	{
		if (list_of_quotes->double_quotes[i] % 2 == 1)
			if (check_for_quotes_none_quotes(list_of_quotes->double_quotes, list_of_quotes->single_quotes, i))
				num_of_content_parts++;
		i++;
	}
	num_of_content_parts = num_of_content_parts + i / 2;
	i = 0;
	while (list_of_quotes->single_quotes[i] != -1)
	{
		if (list_of_quotes->single_quotes[i] % 2 == 1)
			if (check_for_quotes_none_quotes(list_of_quotes->single_quotes, list_of_quotes->double_quotes, i))
				num_of_content_parts++;
		i++;
	}
	num_of_content_parts = num_of_content_parts + i / 2;
	return (num_of_content_parts);
}

t_post_quotes	**make_post_quotes_content(char *str, t_list_of_quotes *list_of_quotes)
{
	t_post_quotes	**content;
	int				index_current_char;
	int				content_i;
	// bool 			expandable;
	// bool 			in_quotes;


	index_current_char = 0;
	content = (t_post_quotes **)malloc(sizeof(t_post_quotes) * (count_how_many_quotes_content_separated(list_of_quotes, str) + 1));
	if (!content)
		exit(1);
	content_i = 0;
	while (str[index_current_char] != '\0')
	{
		if (return_index_until_new(list_of_quotes, index_current_char, NULL, NULL) == index_current_char + 1)
			index_current_char++;
		content[content_i] = (t_post_quotes *)malloc(sizeof(t_post_quotes));
		if (!content[content_i])
			exit(1);
		if (return_index_until_new(list_of_quotes, index_current_char, NULL, NULL) == -1)
		{
			if (str[index_current_char + 1] == '\0')
			{
				free(content[content_i]);
				break ;
			}
			content[content_i]->content = malloc(sizeof(char) * (ft_strlen(str) - index_current_char));
			if (!content[content_i]->content)
				exit(1);
			ft_strlcpy(content[content_i]->content, &str[index_current_char + 1], ft_strlen(str) - index_current_char);
			content[content_i]->have_to_expand = true;
			content[content_i]->is_quotes = false;
			content_i++;
			break ;
		}
		if (str[0] != '\'' && str[0] != '\"' && index_current_char == 0)
		{

			content[content_i]->content = malloc(sizeof(char) * (return_index_until_new(list_of_quotes, index_current_char, NULL, NULL) - index_current_char + 1));
			if (!content[content_i]->content)
				exit(1);
			ft_strlcpy(content[content_i]->content, &str[index_current_char], return_index_until_new(list_of_quotes, index_current_char, &content[content_i]->have_to_expand, &content[content_i]->is_quotes) - index_current_char + 1);
		}
		else
		{
			content[content_i]->content = malloc(sizeof(char) * (return_index_until_new(list_of_quotes, index_current_char, NULL, NULL) - index_current_char));
			if (!content[content_i]->content)
				exit(1);
			ft_strlcpy(content[content_i]->content, &str[index_current_char + 1], return_index_until_new(list_of_quotes, index_current_char, &content[content_i]->have_to_expand, &content[content_i]->is_quotes) - index_current_char);
		}
		index_current_char = return_index_until_new(list_of_quotes, index_current_char, NULL, NULL);
		content_i++;
	}
	content[content_i] = NULL;
	return (content);
}

/*
	This the main of the quotes.
	So if there is not quotes in STR_OG:
	It expand_doll and and doesn't do anything to content, leaving it NULL;
	If there is a quotes:
	It will find them than seperate the content insinde and out side of quotes and puts it into content.

	STR_OF is a pointer to the string, the userinput
	CONTENT is a pointer to the array of pointer of T_POST_QUOTES, (fake return)
*/
void	check_quotes(char **str_og, t_post_quotes ***content)
{
	t_list_of_quotes *list_of_quotes;
	char *str;
	str = (*str_og);

	list_of_quotes = count_and_locate_quotes(str);
	if (list_of_quotes->single_quotes[0] == -1 && list_of_quotes->double_quotes[0] == -1)
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

char	**spit_text_args(char *str, int	*doll_pos);
char	*ft_strjoin_double_str(char **spit_text);
void	split_test_freeur(char **spit_text);
char *get_env_of_valus_str(char *str);

char	*handle_expand_doll(char *str)
{
	int	*doll_pos;
	char **spit_text;
	char *final_str;

	// printf("{%s}\n",str);
	doll_pos = get_doll_position(str);
	if (doll_pos[0] == -1)
		{
			free(doll_pos);
			return (ft_strdup(str));
		}
	spit_text = spit_text_args(str, doll_pos);
	final_str = ft_strjoin_double_str(spit_text);
	split_test_freeur(spit_text);
	free(doll_pos);
	// puts(final_str);
	return (final_str);
}

void	ft_strjoin_double_str_copy_to(char **spit_text, char **str)
{
	int i;
	int j;
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

void	spit_text_args_even(char *str, int	*doll_pos, int i, int num_doll, char **string_split)
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

void	spit_text_args_odd(char *str, int	*doll_pos, int num_doll, char	**temp_str)
{
	if (find_next_space(&str[doll_pos[num_doll]]) == -1)
	{
		if (doll_pos[num_doll + 1] != -1)
			(*temp_str) = ft_strdup_intil_index_n(&str[doll_pos[num_doll] + 1], doll_pos[num_doll + 1] - (doll_pos[num_doll] + 2));
		else
		{
			if (find_next_quote(&str[doll_pos[num_doll] + 1]) - 1 != -2)
				(*temp_str) = ft_strdup_intil_index_n(&str[doll_pos[num_doll] + 1], find_next_quote(&str[doll_pos[num_doll] + 1]) - 1);
			else
			{
				// printf("%d\n", doll_pos[num_doll] + 1);
				(*temp_str) = ft_strdup(&str[doll_pos[num_doll] + 1]);
			}
		}
	}
	else if (doll_pos[num_doll + 1] != -1 && find_next_space(&str[doll_pos[num_doll]] + 1) > doll_pos[num_doll + 1] - (doll_pos[num_doll] + 1))
	{
		// printf("\n %d, %d\n", doll_pos[num_doll + 1],  doll_pos[num_doll]);
		(*temp_str) = ft_strdup_intil_index_n(&str[doll_pos[num_doll] + 1], doll_pos[num_doll + 1] - (doll_pos[num_doll] + 2));
	}
	else
		(*temp_str) = ft_strdup_intil_index_n(&str[doll_pos[num_doll] + 1], find_next_space(&str[doll_pos[num_doll]] + 1) - 1);
}

void	spit_text_args_odd_p2(char	**string_split, char **temp_str, int i)
{
	char *temp;

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

void	spit_text_args_init(int *num_doll, int	*doll_pos, int *total_parts, char	***string_split)
{
	(*num_doll) = 0;
	while (doll_pos[(*num_doll)] != -1)
		(*num_doll)++;
	(*total_parts) = ((*num_doll) * 2) + 1;
	(*string_split) = malloc(sizeof(char *) * ((*total_parts) + 2));
	if (!(*string_split))
		exit(1);
}

/*
	splite the STR into str and the dollars, and also replace the dollar with their value.
	Or for $? make it $! to be catch later. 
	And return the array of str.
*/
char	**spit_text_args(char *str, int	*doll_pos)
{
	int		i;
	char	**string_split;
	int		num_doll;
	char	*temp_str;
	int		total_parts;

	spit_text_args_init(&num_doll, doll_pos, &total_parts, &string_split);
	i = 0;
	num_doll = 0;
	while (i < (total_parts))
	{
		if (i % 2 == 0)
			spit_text_args_even(str, doll_pos, i, num_doll, string_split);
		else
		{
			spit_text_args_odd(str, doll_pos, num_doll, &temp_str);
			spit_text_args_odd_p2(string_split, &temp_str, i);
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
		if (str[i++] == '$')
			if (str[i + 1] != '=')
				num_doll++;
	doll_pos = malloc(sizeof(int) * (num_doll + 1));
	if (!doll_pos)
		exit(EXIT_FAILURE);
	i = 0;
	num_doll = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '$')
			if (str[i + 1] != '=')
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