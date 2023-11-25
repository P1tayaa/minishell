/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_starter.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 22:04:26 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 22:04:34 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
	Initiates values to zero or null and malloc the new lexer i.
	
	LEXER is a pointer to the address of current one
	of double array of t_lexer (&lexer[i])
*/
void	initiate_values_to_zero_null(t_lexer **lexer, int i)
{
	(*lexer) = (t_lexer *)malloc(sizeof(t_lexer));
	if (!(*lexer))
		exit(1);
	(*lexer)->possition = i;
	(*lexer)->args = NULL;
	(*lexer)->cmd = NULL;
	(*lexer)->file = NULL;
	(*lexer)->flags = NULL;
	(*lexer)->tokenid[0] = '\0';
	(*lexer)->execd = false;
}

/*
	Removes the spaces that are at the front and back of
	all parts of LEXER
	by using remove_front_spaces() and remove_back_spaces()
	
	LEXER is a pointer to the address of current one
	of double array of t_lexer (&lexer[i])
*/
void	removes_front_and_back_redondant_spaces(t_lexer **lexer)
{
	(*lexer)->args = remove_front_spaces((*lexer)->args);
	(*lexer)->cmd = remove_front_spaces((*lexer)->cmd);
	(*lexer)->file = remove_front_spaces((*lexer)->file);
	(*lexer)->flags = remove_front_spaces((*lexer)->flags);
	(*lexer)->args = remove_back_spaces((*lexer)->args);
	(*lexer)->cmd = remove_back_spaces((*lexer)->cmd);
	(*lexer)->file = remove_back_spaces((*lexer)->file);
	(*lexer)->flags = remove_back_spaces((*lexer)->flags);
}

/*
	Depending of which one of tokenid (<,>,<<,>>).
	Place the first word of the user input (cmd) to the right place.
	Either in file or args.

	PREVIUS_LEXER is a pointer to the address of previus
	one of double array of t_lexer (&lexer[i - 1])
	
	CURRENT_LEXER is a pointer to the address of current
	one of double array of t_lexer (&lexer[i])
	move_cmd_to_right_place_if_speacific_tokenid
*/
void	move_cmd_if_speacific_tokenid(
		t_lexer **previus_lexer, t_lexer **current_lexer, int i)
{
	if (i > 0)
	{
		if ((*previus_lexer)->tokenid[0]
			== '<' && (*previus_lexer)->tokenid[1] == '\0')
			move_file_name_to_file_and_comand_back(
				(*previus_lexer), (*current_lexer));
		if ((*previus_lexer)->tokenid[0]
			== '<' && (*previus_lexer)->tokenid[1] == '<')
		{
			(*previus_lexer)->args = (*current_lexer)->cmd;
			(*current_lexer)->cmd = NULL;
		}
		if ((*previus_lexer)->tokenid[0] == '>')
		{
			(*previus_lexer)->file = (*current_lexer)->cmd;
			(*current_lexer)->cmd = NULL;
		}
	}
}

/*
	Gets the string user input and seperates all info of it into
	the struct t_lexer for to then be executated
	
	STR is the user input
*/
t_lexer	**main_parser(char *str)
{
	int		token_num;
	int		curser;
	int		i;
	t_lexer	**lexer;

	token_num = count_token(str);
	lexer = (t_lexer **)malloc(sizeof(t_lexer *) * (token_num + 2));
	if (!lexer)
		exit(1);
	i = 0;
	curser = 0;
	while (i <= token_num)
	{
		initiate_values_to_zero_null(&lexer[i], i);
		curser = parse_until_token_id(str, i, lexer, curser);
		if (ft_char_find(lexer[i]->args, "-") != -1)
			move_flags_from_args_to_flags(lexer[i]);
		move_cmd_if_speacific_tokenid(
			&lexer[i - 1], &lexer[i], i);
		removes_front_and_back_redondant_spaces(&lexer[i]);
		i++;
	}
	lexer[i] = NULL;
	i = 0;
	return (lexer);
}

// count num of dash and flags char len
void	count_num_dash_and_flags_char_len(char **str,
		int i, int *num_of_dash, int *flags_char_len)
{
	int	j;

	while ((*str)[i] != '\0')
	{
		if (ft_isspace((*str)[i]) == true || i == 0)
		{
			if (ft_isspace((*str)[i]))
				i++;
			if ((*str)[i] == '\0')
				break ;
			if ((*str)[i] == '-')
			{
				(*num_of_dash)++;
				j = 0;
				while ((*str)[i + j] != '\0' &&
					ft_isspace((*str)[i + j]) == false)
					j++;
				i = i + j;
				(*flags_char_len) = (*flags_char_len) + j;
				continue ;
			}
		}
		i++;
	}
}
