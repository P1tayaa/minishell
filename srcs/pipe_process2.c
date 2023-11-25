/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_process2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 23:15:24 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 23:24:51 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	initialize_builtins(const char **builtins);

int	is_built_in(t_lexer **lexer, int lex_count)
{
	int			i;
	int			cmd_len;
	int			builtin_len;
	const char	*builtins[7];

	i = 0;
	initialize_builtins(builtins);
	while (builtins[i])
	{
		cmd_len = ft_strlen(lexer[lex_count]->cmd);
		builtin_len = ft_strlen(builtins[i]);
		if (cmd_len > builtin_len)
		{
			if (ft_strncmp(lexer[lex_count]->cmd,
					builtins[i], builtin_len) == 0)
				return (1);
		}
		else if (ft_strncmp(lexer[lex_count]->cmd, builtins[i], cmd_len) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	collapse_lexer(t_lexer **lexer)
{
	int	collapse;
	int	len_collapse;
	int	len_next;

	collapse = 0;
	while (lexer[collapse] != NULL)
	{
		if (lexer[collapse + 1] != NULL)
		{
			len_collapse = ft_strlen(lexer[collapse]->cmd);
			len_next = ft_strlen(lexer[collapse + 1]->cmd);
			if ((len_collapse < len_next && ft_memcmp(lexer[collapse]->cmd,
						lexer[collapse + 1]->cmd, len_collapse) == 0) || (
					len_collapse >= len_next && ft_memcmp (lexer[
							collapse]->cmd, lexer[collapse + 1]->cmd,
						len_next) == 0))
				collapse++;
			else
				break ;
		}
		else
			break ;
	}
	return (collapse);
}

int	are_all_commands_thesame(t_lexer **lexer)
{
	int	collapse;
	int	i;

	i = 0;
	collapse = 0;
	while (lexer[i] != NULL)
		i++;
	if (i < 10)
		return (0);
	collapse = collapse_lexer(lexer);
	if (collapse != i + 1)
		return (1);
	else
		return (0);
}

void	manage_collapse(t_lexer **lexer, t_pipedata *data, int *pid, int doll)
{
	if (lexer[(*data).lex_count]->cmd != NULL && lexer[(*data).lex_count]->execd
		== false && lexer[(*data).lex_count]->tokenid[0] != '<'
		&& lexer[(*data).lex_count]->tokenid[0] != '>')
	{
		if (lexer[(*data).lex_count + 1] != NULL)
		{
			if (are_all_commands_thesame(lexer) == 1)
			{
				lexer[(*data).lex_count]->args
					= doll_to_num(lexer[(*data).lex_count]->args, doll);
				(*pid) = fork();
				if ((*pid) == 0)
				{
					execute_child_process(data);
					exit(EXIT_SUCCESS);
				}
				lexer[(*data).lex_count]->execd = true;
				waitpid((*pid), &doll, 0);
				if WIFEXITED(doll)
					doll = WEXITSTATUS(doll);
			}
		}
	}
}

void	wait_process_signals(int *doll, int *pid)
{
	waitpid((*pid), &(*doll), 0);
	if ((*doll) == 131 || (*doll) == 3)
	{
		ft_putstr_fd("Quit (core dumped)\n", 2);
		g_exit_status = 131;
	}
	else if (WIFEXITED((*doll)))
		g_exit_status = WEXITSTATUS((*doll));
	while (wait(NULL) != -1)
		;
}
