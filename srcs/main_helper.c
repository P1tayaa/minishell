/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 22:46:06 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 22:51:46 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	export_andle_no_quotes(t_lexer ***lexer);
bool	check_unset_noquotes(t_lexer ***lexer);
t_lexer	**parser_with_quotes(t_post_quotes **content);
bool	check_export_for_quotes(t_post_quotes	***content, t_lexer ***lexer);
bool	check_unset_for_quotes(t_post_quotes	***content, t_lexer ***lexer);
void	free_content(t_post_quotes **content);

void	finish_output(t_lexer **lexer)
{
	if (ft_strlen(lexer[0]->cmd) == 3)
		if ((ft_memcmp(lexer[0]->cmd, "cat", 3) == 0)
			&& lexer[1] == NULL && lexer[0]->args == NULL)
			write(2, "\033[18D\033[K", 8);
}

int	noquotes_into_pipes(t_lexer ***lexer)
{
	if (export_andle_no_quotes(lexer))
		return (1);
	if (check_unset_noquotes(lexer))
		return (1);
	else
		piping((*lexer));
	return (0);
}

int	lexer_with_quotes(t_lexer ***lexer, t_post_quotes ***content, char **str)
{
	(*lexer) = parser_with_quotes((*content));
	if (check_export_for_quotes(content, lexer)
		|| check_unset_for_quotes(content, lexer))
	{
		if ((*str))
		{
			free((*str));
			(*str) = NULL;
		}
		return (1);
	}
	free_content((*content));
	return (0);
}

char	*read_user_input(bool quotes_test)
{
	char	*str;
	char	*str_return;

	rl_catch_signals = 0;
	while (1)
	{
		str = readline("minishell_OS_1.0$ ");
		if (str == NULL)
		{
			printf("\n");
			break ;
		}
		if (!quotes_test && str && *str)
			add_history(str);
		break ;
	}
	if (str == NULL)
		return (NULL);
	str_return = ft_strdup(str);
	if (str)
		free(str);
	return (str_return);
}

char	is_prompt_empty(char *str)
{
	int		i;
	char	is_valid;

	i = 0;
	is_valid = 0;
	while (str[i])
		i++;
	if (i > 0)
		is_valid = 1;
	return (is_valid);
}
