/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 23:16:48 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 23:22:42 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	initialize_pipedata(t_pipedata *data)
{
	extern char	**environ;

	data->input_fd = -1;
	data->lex_count = 0;
	data->og_in = dup(STDIN_FILENO);
	data->og_out = dup(STDOUT_FILENO);
	data->environ = &environ;
}

int	are_there_previous_cmds(t_lexer **lexer, t_pipedata *data)
{
	int	i;

	i = (*data).lex_count;
	while (i >= 0)
	{
		if (lexer[i] != NULL)
		{
			if (lexer[i]->cmd != NULL)
				return (i);
		}
		i--;
	}
	return (-1);
}

// 0 default
// 1 ignore
// 2 heredoc
// 3 child

int	manage_heredoc(t_pipedata *data, t_lexer **lexer)
{
	char	*str;

	rl_catch_signals = 0;
	str = NULL;
	if (lexer[(*data).lex_count]->tokenid[1] == '<')
	{
		manage_signals(2);
		str = here_doc_starter(lexer[(*data).lex_count]->args);
		lexer[(*data).lex_count]->args = sjoin_fr(
				lexer[(*data).lex_count]->args, ft_strdup("heredoc.txt"));
		printf("args = %s\n", lexer[(*data).lex_count]->args);
		(*data).fd[1] = open(lexer[(*data).lex_count]->args,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
		write((*data).fd[1], str, ft_strlen(str));
		if (lexer[(*data).lex_count + 1]->cmd != NULL)
			dup2((*data).fd[1], STDOUT_FILENO);
		free(str);
		(*data).lex_count = are_there_more_cmds(lexer, (*data).lex_count);
		if ((*data).lex_count == 0)
			return (manage_signals(0), (*data).lex_count);
		else
			return (manage_signals (0), 1);
	}
	return (manage_signals(0), 0);
}

void	manage_redirection(t_pipedata *data, t_lexer **lexer, int i)
{
	(*data).fd[0] = redirection_handler(lexer[i]);
	dup2((*data).fd[0], STDIN_FILENO);
	if (are_there_more_cmds(lexer, (*data).lex_count) != 0)
		dup2((*data).fd[1], STDOUT_FILENO);
	if (lexer[(*data).lex_count + 1] != NULL)
	{
		if (lexer[(*data).lex_count + 1]->tokenid[0] == '>')
		{
			if (lexer[(*data).lex_count + 2] != NULL)
			{
				if (lexer[(*data).lex_count + 2]->tokenid[0] != '>')
				{
					(*data).fd[1] = redirection_handler(
							lexer[(*data).lex_count + 1]);
					dup2((*data).fd[1], STDOUT_FILENO);
				}
			}
			else
			{
				(*data).fd[1] = redirection_handler(
						lexer[(*data).lex_count + 1]);
				dup2((*data).fd[1], STDOUT_FILENO);
			}
		}
	}
}

void	found_last_out(t_lexer **lexer, t_pipedata *data, int *out_files)
{
	if (lexer[(*out_files) - 1]->tokenid[0] == '>')
	{
		(*data).fd[1] = redirection_handler(lexer[(*out_files)]);
		dup2((*data).fd[1], STDOUT_FILENO);
	}
}
