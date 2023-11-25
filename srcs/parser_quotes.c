/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 22:00:07 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 22:21:35 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	parse_with_quotes_none_quotes_assigning_if_token(char
		**curent_content, int *j, t_lexer ***lexer, int *i)
{
	write_the_right_token((*j), (*curent_content), ((*lexer)[(*i)]->tokenid));
	(*j) = (*j) + ft_strlen((*lexer)[(*i)]->tokenid);
	if ((*lexer)[(*i)]->flags[1] == '\0')
	{
		free((*lexer)[(*i)]->flags);
		(*lexer)[(*i)]->flags = NULL;
	}
	if ((*lexer)[(*i)]->args != NULL)
	{
		(*lexer)[(*i)]->args = remove_front_spaces((*lexer)[(*i)]->args);
		(*lexer)[(*i)]->args = remove_back_spaces((*lexer)[(*i)]->args);
	}
	if ((*lexer)[(*i)]->cmd != NULL)
		(*lexer)[(*i)]->cmd = remove_back_spaces((*lexer)[(*i)]->cmd);
	move_cmd_if_speacific_tokenid(
		&(*lexer)[(*i - 1)], &(*lexer)[(*i)], (*i));
	(*i)++;
	if ((*curent_content)[(*j)] != '\0')
	{
		initiate_values_to_zero_null(&(*lexer)[(*i)], (*i));
		(*lexer)[(*i)]->flags = ft_strdup("-");
		if (!(*lexer)[(*i)]->flags)
			exit(1);
	}
}

void	move_flags_for_quotes(char **curent_content,
			int *j, int i, t_lexer ***lexer);

void	parse_with_quotes_none_quotes_assigning(char
		**curent_content, bool *function_done, t_lexer ***lexer, int *i)
{
	int	j;

	j = 0;
	while ((*curent_content)[j] != '\0')
	{
		if ((*function_done) == false
			&& ft_isspace((*curent_content)[j]) == false)
		{
			j = copy_until_space(j, (*curent_content), &(*lexer)[(*i)]->cmd);
			(*function_done) = true;
		}
		if (is_a_token_id(&(*curent_content)[j]) == true)
		{
			parse_with_quotes_none_quotes_assigning_if_token(
				curent_content, &j, lexer, i);
			(*function_done) = false;
		}
		if ((*function_done) && ft_isspace((*curent_content)[j]) == false)
		{
			move_flags_for_quotes(curent_content, &j, (*i), &(*lexer));
		}
		if (ft_isspace((*curent_content)[j]))
			j++;
	}
}

// malloc the content
void	parse_with_quotes_init(t_post_quotes **content,
		int	*token_num, t_lexer ***lexer)
{
	int	i;

	i = 0;
	(*token_num) = 0;
	while (content[i] != NULL)
	{
		if (content[i]->is_quotes == false)
			(*token_num) = (*token_num) + count_token(content[i]->content);
		i++;
	}
	(*lexer) = (t_lexer **)malloc(sizeof(t_lexer *) * ((*token_num) + 2));
	if (!(*lexer))
		exit(1);
}

void	parse_with_quotes_quotes_assigning(t_post_quotes **content,
			bool *function_done, t_lexer ***lexer, int i);

void	parser_with_quotes_p2(t_post_quotes **content,
		bool *function_done, t_lexer ***lexer, int *i)
{
	int	i_content;

	i_content = 0;
	while (content[i_content] != NULL)
	{
		if (content[i_content]->is_quotes)
			parse_with_quotes_quotes_assigning(
				&content[i_content++], function_done, lexer, (*i));
		else
			parse_with_quotes_none_quotes_assigning(
				&content[i_content++]->content, function_done, lexer, i);
	}
	move_cmd_if_speacific_tokenid(&(*lexer)[(*i - 1)], &(*lexer)[(*i)], (*i));
	if ((*lexer)[(*i)]->args != NULL)
	{
		(*lexer)[(*i)]->args = remove_front_spaces((*lexer)[(*i)]->args);
		(*lexer)[(*i)]->args = remove_back_spaces((*lexer)[(*i)]->args);
	}
	if ((*lexer)[(*i)]->cmd != NULL)
		(*lexer)[(*i)]->cmd = remove_back_spaces((*lexer)[(*i)]->cmd);
	if ((*lexer)[(*i)]->flags[1] == '\0')
	{
		free((*lexer)[(*i)]->flags);
		(*lexer)[(*i)]->flags = NULL;
	}
}

/*
	Main parser when there are quotes
	take the content of input, and put the info in the right parts of a lexer.
	return that lexeer
*/
t_lexer	**parser_with_quotes(t_post_quotes **content)
{
	t_lexer	**lexer;
	int		token_num;
	int		i;
	bool	function_done;

	parse_with_quotes_init(content, &token_num, &lexer);
	i = 0;
	function_done = false;
	if (function_done == false)
	{
		initiate_values_to_zero_null(&lexer[i], i);
		lexer[i]->flags = ft_strdup("-");
		if (!lexer[i]->flags)
			exit(1);
	}
	parser_with_quotes_p2(content, &function_done, &lexer, &i);
	i++;
	lexer[i] = NULL;
	return (lexer);
}
