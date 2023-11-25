/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 13:30:47 by sboulain          #+#    #+#             */
/*   Updated: 2023/11/25 22:06:04 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		count_pipes(char *str);
char	**get_list_of_tokenid(void);
void	free_double_array(char **list_of_tokenid);
int		count_token(char *input);
void	free_parser(t_lexer **lexer);
void	free_lexer(t_lexer **lexer);
int		parse_until_token_id(char *input, int current_lex,
			t_lexer **lexer, int curser);
int		ft_char_find(char *str, const char *list_of_char);
bool	ft_isspace(unsigned char c);

void	count_flags_char_len(int *i, int *flags_char_len, t_lexer *lexer)
{
	while (!ft_isspace(lexer->args[(*i)]) && lexer->args[(*i)] != '\0')
	{
		(*flags_char_len)++;
		(*i)++;
	}
	(*flags_char_len)++;
}

/*
	Return the number of charaters that are going to be moved from arg to flags,
	And keeps track of how many are going to be left in arg with ARGS_CHAR_LEN
*/
int	count_number_flags_char(int *args_char_len, t_lexer *lexer)
{
	int	flags_char_len;
	int	i;

	flags_char_len = 0;
	*args_char_len = 0;
	i = 0;
	while (lexer->args[i] != '\0')
	{
		if (i != 0)
		{
			if (ft_isspace(lexer->args[i - 1]) && lexer->args[i] == '-')
			{
				count_flags_char_len(&i, &flags_char_len, lexer);
				continue ;
			}
		}
		if (i == 0 && lexer->args[i] == '-')
			count_flags_char_len(&i, &flags_char_len, lexer);
		else
		{
			(*args_char_len)++;
			i++;
		}
	}
	return (flags_char_len);
}

/*
	Malloc in new args and flags with the pointer
	to the address given in ARGS and FLAGS
*/
void	malloc_new_args_and_new_flags(int args_char_len, char **args,
		char **flags, int flags_char_len)
{
	if (args_char_len != 0)
	{
		(*args) = malloc(sizeof(char) * (args_char_len + 1));
		if (!(*args))
			exit (-1);
	}
	else
		(*args) = NULL;
	(*flags) = malloc(sizeof(char) * (flags_char_len + 2));
	if (!(*flags))
		exit (-1);
}

void	move_flag_from_arg_to_flags(int *i, int *flags_char_len,
		t_lexer *lexer, char **flags)
{
	(*i)++;
	while (!ft_isspace(lexer->args[(*i)]) && lexer->args[(*i)] != '\0')
	{
		(*flags)[(*flags_char_len)] = lexer->args[(*i)];
		(*flags_char_len)++;
		(*i)++;
	}
}

void	move_all_flag_from_arg_to_flags(char **flags,
		int args_char_len, char **args, t_lexer *lexer)
{
	int	i;
	int	flags_char_len;

	i = 0;
	flags_char_len = 1;
	while (lexer->args[i] != '\0')
	{
		if (i != 0)
		{
			if (ft_isspace(lexer->args[i - 1]) && lexer->args[i] == '-')
			{
				move_flag_from_arg_to_flags(&i, &flags_char_len, lexer, flags);
				continue ;
			}
		}
		if (i == 0 && lexer->args[i] == '-')
		{
			move_flag_from_arg_to_flags(&i, &flags_char_len, lexer, flags);
			continue ;
		}
		(*args)[args_char_len++] = lexer->args[i++];
	}
	(*flags)[flags_char_len] = '\0';
	if ((*args) != NULL)
		(*args)[args_char_len] = '\0';
}
