/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_nn.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 22:05:49 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 22:20:59 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	move_all_flag_from_arg_to_flags(char **flags,
			int args_char_len, char **args, t_lexer *lexer);
int		count_number_flags_char(int *args_char_len, t_lexer *lexer);

/*
	Move the flags from args to there respective
	par of the lexer by reconising where the '-'
	
*/
void	move_flags_from_args_to_flags(t_lexer *lexer)
{
	char	*args;
	char	*flags;
	int		flags_char_len;
	int		args_char_len;
	char	*temp;

	flags_char_len = count_number_flags_char(&args_char_len, lexer);
	malloc_new_args_and_new_flags(args_char_len, &args, &flags, flags_char_len);
	flags[0] = '-';
	move_all_flag_from_arg_to_flags(&flags, 0, &args, lexer);
	free(lexer->args);
	lexer->args = args;
	if (lexer->flags != NULL)
	{
		temp = ft_strjoin(lexer->flags, flags);
		free(lexer->flags);
		free(flags);
		lexer->flags = temp;
	}
	else
		lexer->flags = flags;
}

void	parse_quote_cmd(t_post_quotes **content,
			t_lexer ***lexer, bool *function_done, int i);

/*
depending on the current situation place
the content of the quotes in the right place
*/
void	parse_with_quotes_quotes_assigning(t_post_quotes **content,
		bool *function_done, t_lexer ***lexer, int i)
{
	if ((*function_done) == false)
		parse_quote_cmd(content, lexer, function_done, i);
	else
	{
		if ((*content)->content[0] == '-')
		{
			if ((*content)->have_to_expand)
				(*lexer)[i]->flags = sjoin_fr((*lexer)[i]->flags,
						handle_expand_doll(&(*content)->content[1]));
			else
				(*lexer)[i]->flags = sjoin_fr((*lexer)[i]->flags,
						ft_strdup(&(*content)->content[1]));
		}
		else
		{
			if ((*content)->have_to_expand)
				(*lexer)[i]->args = sjoin_fr((*lexer)[i]->args,
						sjoin_fr(handle_expand_doll((*content)->content),
							ft_strdup(" ")));
			else
				(*lexer)[i]->args = sjoin_fr((*lexer)[i]->args, sjoin_fr(
							ft_strdup((*content)->content), ft_strdup(" ")));
		}
	}
}

char	*get_flags_str(char **str);

// not sure if done right
void	move_flags_for_quotes(char **curent_content,
		int *j, int i, t_lexer ***lexer)
{
	char	*temp;
	char	*temp2;

	temp = NULL;
	(*j) = copy_until_tokenid(*j, (*curent_content), &temp);
	if (temp != NULL)
	{
		printf("temp == (%s)\n", temp);
		temp2 = get_flags_str(&temp);
		(*lexer)[i]->args = sjoin_fr((*lexer)[i]->args,
				sjoin_fr(temp, ft_strdup(" ")));
		(*lexer)[i]->flags = sjoin_fr((*lexer)[i]->flags, temp2);
	}
}
