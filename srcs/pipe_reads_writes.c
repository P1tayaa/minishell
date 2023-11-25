/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_reads_writes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 23:16:11 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 23:25:05 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	found_last_out(t_lexer **lexer, t_pipedata *data, int *out_files);

void	manage_multiple_outs(t_pipedata *data, t_lexer **lexer, int *out_files)
{
	int	temp_fd;

	temp_fd = 0;
	(*out_files) = (*data).lex_count;
	while (lexer[(*out_files)] != NULL)
	{
		if (lexer[(*out_files) + 1] != NULL)
		{
			if (lexer[(*out_files) + 1]->tokenid[0] == '>')
			{
				temp_fd = open(lexer[(*out_files)]->file,
						O_WRONLY | O_CREAT | O_APPEND, 0644);
				close(temp_fd);
				(*out_files)++;
			}
			else
				break ;
		}
	}
	if ((*out_files) != 0)
		found_last_out(lexer, data, out_files);
}

void	manage_outwrite(t_lexer **lexer, t_pipedata *data, int *out_files)
{
	if (lexer[0]->tokenid[0] == '<')
	{
		if (lexer[1] != NULL)
		{
			if (ft_memcmp(lexer[1]->tokenid, "|", 1) == 0)
			{
				(*data).fd[1] = redirection_handler
					(lexer[(*data).lex_count]);
				dup2((*data).fd[1], STDOUT_FILENO);
			}
		}
	}
	else if ((*out_files) == (*data).lex_count)
	{
		(*data).fd[1] = redirection_handler(lexer[(*data).lex_count]);
		dup2((*data).fd[1], STDOUT_FILENO);
		dup2((*data).input_fd, STDIN_FILENO);
	}
}

void	manage_non_redirections(t_pipedata *data, t_lexer **lexer)
{
	if ((*data).lex_count != 0 && lexer[(*data).lex_count + 1] != NULL)
	{
		dup2((*data).input_fd, STDIN_FILENO);
		dup2((*data).fd[1], STDOUT_FILENO);
	}
	else if ((*data).lex_count != 0 || (*data).input_fd != -1)
		dup2((*data).input_fd, STDIN_FILENO);
	else
		dup2((*data).fd[1], STDOUT_FILENO);
}

void	close_fds(t_pipedata *data)
{
	close((*data).input_fd);
	close((*data).fd[0]);
	close((*data).fd[1]);
}

void	manage_general_outs(t_pipedata *data, t_lexer **lexer, int *out_files)
{
	manage_multiple_outs(data, lexer, out_files);
	manage_outwrite(lexer, data, out_files);
}
