/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 23:14:44 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 23:24:36 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	wait_process_signals(int *doll, int *pid);
int		manage_reads_writes(t_pipedata *data, t_lexer **lexer);
void	manage_collapse(t_lexer **lexer, t_pipedata *data, int *pid, int doll);

void	manage_single_execs(t_lexer **lexer,
		t_pipedata *data, int *pid, int *doll)
{
	if (lexer[(*data).lex_count]->tokenid[0]
		!= '<' && is_built_in(lexer, (*data).lex_count) == 1)
	{
		if (lexer[1] == NULL)
		{
			lexer[(*data).lex_count]->args
				= doll_to_num(lexer[(*data).lex_count]->args, (*doll));
			(*doll) = executer(lexer, data);
			lexer[(*data).lex_count]->execd = true;
		}
	}
	if (lexer[1] == NULL && lexer[(*data).lex_count]->execd == false)
	{
		(*pid) = fork();
		if ((*pid) == 0)
		{
			lexer[(*data).lex_count]->args
				= doll_to_num(lexer[(*data).lex_count]->args, (*doll));
			manage_signals(3);
			execute_child_process(data);
			manage_signals(0);
			exit(EXIT_SUCCESS);
		}
		wait_process_signals(doll, pid);
	}
}

void	create_and_run_child(t_pipedata *data,
		t_lexer **lexer, int *pid, int *doll)
{
	manage_signals(1);
	lexer[(*data).lex_count]->args
		= doll_to_num(lexer[(*data).lex_count]->args, (*doll));
	pipe((*data).fd);
	(*pid) = fork();
	if ((*pid) == -1)
	{
		perror("Fork failed");
		exit(EXIT_FAILURE);
	}
	if ((*pid) == 0)
	{
		manage_signals(3);
		if (manage_reads_writes(&(*data), lexer) == -1)
			return ;
		if (lexer[(*data).lex_count]->cmd)
			execute_child_process(&(*data));
	}
	else
		(*doll) = parent_management(&(*data), lexer, (*pid));
	manage_signals(0);
}

void	pipes(t_lexer **lexer, t_pipedata *data, int *pid, int *doll)
{
	if (lexer[1] != NULL)
	{
		while (lexer[(*data).lex_count] && !lexer[(*data).lex_count]->execd)
		{
			if (lexer[(*data).lex_count]->cmd != NULL)
			{
				if (ft_memcmp(lexer[(*data).lex_count]->cmd, "cat", 3) == 0)
				{
					if (lexer[(*data).lex_count]->args == NULL)
					{
						if (lexer[((*data).lex_count++) + 1] != NULL)
						{
							readline("\n");
							continue ;
						}
					}
				}
			}
			create_and_run_child(data, lexer, pid, doll);
			if (lexer[(*data).lex_count + 1] == NULL
				|| are_there_more_cmds(lexer, (*data).lex_count) == 0)
				break ;
			(*data).lex_count = are_there_more_cmds(lexer, (*data).lex_count);
		}
	}
}

void	manage_single_cmds(t_lexer **lexer,
	t_pipedata *data, int *pid, int *doll)
{
	manage_collapse(lexer, data, pid, (*doll));
	manage_single_execs(lexer, data, pid, doll);
}

void	piping(t_lexer **lexer)
{
	t_pipedata			data;
	int					pid;
	static int			doll;
	char				*temp;

	initialize_pipedata(&data);
	if (lexer[0] == NULL)
		return ;
	data.lexer = lexer;
	manage_single_cmds(lexer, &data, &pid, &doll);
	pipes(lexer, &data, &pid, &doll);
	if (lexer[0]->tokenid[0] == '<')
	{
		if (lexer[0]->tokenid[1] == '<')
		{
			temp = sjoin_fr(ft_strdup(lexer[0]->args),
					ft_strdup("heredoc.txt"));
			unlink(temp);
			free(temp);
		}
	}
	dup2(data.og_out, STDOUT_FILENO);
	dup2(data.og_in, STDIN_FILENO);
	close(data.og_out);
	close(data.og_in);
}
