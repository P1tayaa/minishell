/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_check_quotes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 22:25:40 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 22:43:40 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void				chekc_quotes_and_remove_fake_quotes(
						t_list_of_quotes **list_of_quotes, char **str);
t_post_quotes		**make_post_quotes_content(char *str,
						t_list_of_quotes *list_of_quotes);

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
void	check_quotes_diff(char **str_og, t_post_quotes ***content)
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
int					*get_doll_position(char *str);

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
