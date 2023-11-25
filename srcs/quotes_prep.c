/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_prep.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 22:26:15 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 22:45:15 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		count_how_many_quotes_content_separated(
			t_list_of_quotes *list_of_quotes, char *str);
void	add_the_end_content(char *str,
			t_post_quotes ***content, int index_current_char, int *content_i);

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
