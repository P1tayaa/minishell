/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 21:58:54 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 21:59:03 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	write_the_right_token(int i, char *input_after_curser, char tokenid[3])
{
	int	j;

	j = 0;
	while (input_after_curser[i + j] != '\0'
		&& !ft_isspace(input_after_curser[i + j])
		&& is_a_token_id(&input_after_curser[i + j]) && j < 2)
	{
		(tokenid)[j] = input_after_curser[i + j];
		j++;
	}
	(tokenid)[j] = '\0';
	return (i + j);
}

/*
	Puts the right values in the LEXER at index CURRENT_LEX, with the string INPUT.
	And continue reading the INPUT starting at the curser.
	and return the new Curser (index at whitch it is done reading)
*/
int	parse_until_token_id(
		char *input, int current_lex, t_lexer **lexer, int curser)
{
	int		i;
	bool	function_done;

	i = 0;
	function_done = false;
	while (input[i + curser] != '\0')
	{
		if (function_done == false && ft_isspace(input[i + curser]) == false)
		{
			i = copy_until_space(i, &input[curser], &lexer[current_lex]->cmd);
			function_done = true;
		}
		if (function_done && ft_isspace(input[i + curser]) == false)
			i = copy_until_tokenid(i,
					&input[curser], &(lexer[current_lex]->args));
		if (is_a_token_id(&input[i + curser]) == true)
		{
			i = write_the_right_token(i,
					&input[curser], (lexer[current_lex]->tokenid));
			break ;
		}
		if (ft_isspace(input[i + curser]))
			i++;
	}
	return (curser + i);
}

int	count_char_until_next_token(char *input)
{
	char	**list_of_tokenid;
	int		i;
	int		j;
	int		k;

	list_of_tokenid = get_list_of_tokenid();
	i = 0;
	while (input[i] != '\0')
	{
		j = 0;
		while (list_of_tokenid[j] != NULL)
		{
			k = 0;
			while (list_of_tokenid[j][k] == input[i + k])
				k++;
			if (list_of_tokenid[j][k] == '\0')
			{
				free_double_array(list_of_tokenid);
				return (i);
			}
			j++;
		}
		i++;
	}
	return (free_double_array(list_of_tokenid), i);
}

int	set_k_for_count_token(char *input, int i, char	**list_of_tokenid, int j)
{
	int		k;

	k = 0;
	while (input[i + k] != '\0' && list_of_tokenid[j][k] == input[i + k])
		k++;
	return (k);
}

/*
	Goes threw string input and return the number of tokenID
	tokenid: ||, |, <<, <, >>, >

	PS: I think this is lower then n^2 complexity, log(n) I think
*/
int	count_token(char *input)
{
	char	**list_of_tokenid;
	int		num_of_token;
	int		i;
	int		j;
	int		k;

	list_of_tokenid = get_list_of_tokenid();
	num_of_token = 0;
	i = -1;
	while (input[++i] != '\0')
	{
		j = 0;
		while (list_of_tokenid[j] != NULL)
		{
			k = set_k_for_count_token(input, i, list_of_tokenid, j);
			if (list_of_tokenid[j][k] == '\0')
			{
				i = i + k;
				num_of_token++;
				break ;
			}
			j++;
		}
	}
	return (free_double_array(list_of_tokenid), num_of_token);
}
