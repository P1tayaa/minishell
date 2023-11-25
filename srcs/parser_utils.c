/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 22:01:03 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 22:19:47 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		count_char_until_next_token(char *input);

/*
	Return true if INPUT is of these:
	tokenid: ||, |, <<, <, >>, >
*/
bool	is_a_token_id(char *input)
{
	char	**list_of_tokenid;
	int		i;
	int		j;

	list_of_tokenid = get_list_of_tokenid();
	i = 0;
	while (list_of_tokenid[i] != NULL)
	{
		j = 0;
		while (input[j] != '\0' && list_of_tokenid[i][j] == input[j])
		{
			j++;
		}
		if (list_of_tokenid[i][j] == '\0')
		{
			free_double_array(list_of_tokenid);
			return (true);
		}
		i++;
	}
	free_double_array(list_of_tokenid);
	return (false);
}

/*
Copies from index I from INPUT_AFTER_CURSE
into pointer to the string DESTINATION
But only copies until the end of the string or
until a space or until a tokenid.
And return the index at whitch it stoped.
*/
int	copy_until_space(int i, char *input_after_curser, char **destination)
{
	int	j;

	j = 0;
	while (input_after_curser[i + j] != '\0'
		&& (!ft_isspace(input_after_curser[i + j])
			&& !is_a_token_id(&input_after_curser[i + j])))
	{
		j++;
	}
	if (j == 0)
		return (i + j);
	(*destination) = (char *)malloc(sizeof(char) * (j + 2));
	if (!(*destination))
		exit(1);
	j = 0;
	while (input_after_curser[i + j] != '\0')
	{
		if ((ft_isspace(input_after_curser[i + j])
				|| is_a_token_id(&input_after_curser[i + j])))
			break ;
		(*destination)[j] = input_after_curser[i + j];
		j++;
	}
	(*destination)[j] = '\0';
	return (i + j);
}

/*
	Copies from index I from INPUT_AFTER_CURSER
	into pointer to the string DESTINATION
	But only copies until the end of the string
	or until a tokenid.
	And return the index at whitch it stoped.
*/
int	copy_until_tokenid(int i, char *input_after_curser, char **destination)
{
	int	j;

	j = 0;
	while (input_after_curser[i + j] != '\0')
	{
		if (is_a_token_id(&input_after_curser[i + j]))
			break ;
		j++;
	}
	if (j == 0)
		return (i + j);
	(*destination) = (char *)malloc(sizeof(char) * (j + 2));
	if (!(*destination))
		exit(1);
	j = 0;
	while (input_after_curser[i + j] != '\0')
	{
		if (is_a_token_id(&input_after_curser[i + j]))
			break ;
		(*destination)[j] = input_after_curser[i + j];
		j++;
	}
	(*destination)[j] = '\0';
	return (i + j);
}

char	*init_new_flag_str(int flags_char_len, char **str, int num_of_dash);
void	null_terminate_flags_str(int flags_char_len,
			char **new_str, char **str, char **flags);

char	*get_flags_str(char **str)
{
	char	*flags;
	char	*new_str;
	int		flags_char_len;
	int		i;
	int		num_of_dash;

	if (str == NULL)
		return (NULL);
	i = 0;
	flags_char_len = 0;
	num_of_dash = 0;
	new_str = NULL;
	count_num_dash_and_flags_char_len(str, i, &num_of_dash, &flags_char_len);
	if (flags_char_len == 0)
		return (NULL);
	flags = malloc(sizeof(char) * (flags_char_len + 1));
	if (!flags)
		exit (1);
	new_str = init_new_flag_str(flags_char_len, str, num_of_dash);
	null_terminate_flags_str(flags_char_len, &new_str, str, &flags);
	return (flags);
}
// ! this is for lexer_quotes

char	*sjoin_fr(char *s1, char *s2);
char	*handle_expand_doll(char *str);

bool	is_a_token_id(char *input);
int		copy_until_tokenid(int i, char *input_after_curser, char **destination);
int		copy_until_space(int i, char *input_after_curser, char **destination);
int		write_the_right_token(int i, char *input_after_curser, char tokenid[3]);

void	parse_quote_cmd(t_post_quotes **content,
		t_lexer ***lexer, bool *function_done, int i)
{
	if ((*content)->have_to_expand)
		(*lexer)[i]->cmd = handle_expand_doll((*content)->content);
	else
		(*lexer)[i]->cmd = ft_strdup((*content)->content);
	(*function_done) = true;
}
