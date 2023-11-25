/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_helper2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 23:17:27 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 23:17:32 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	initialize_builtins(const char **builtins)
{
	builtins[0] = "pwd";
	builtins[1] = "echo";
	builtins[2] = "exit";
	builtins[3] = "clear";
	builtins[4] = "cd";
	builtins[5] = "env";
	builtins[6] = NULL;
}

int	are_there_more_cmds(t_lexer **lexer, int current)
{
	current++;
	while (lexer[current] != NULL)
	{
		if (lexer[current]->cmd != NULL)
			return (current);
		current++;
	}
	return (0);
}

int	pipe_export_error(t_lexer ***lexer)
{
	if (((*lexer))[1] != NULL && ((*lexer))[0]->args != NULL)
	{
		write(2, "Can't pipe export when setting value\n", 37);
		lexer_free(((*lexer)));
		return (1);
	}
	return (0);
}

void	export_parent_management(t_lexer ***lexer, t_pipedata *data, int pid)
{
	int	status;

	status = 0;
	if (are_there_more_cmds((*lexer), (*data).lex_count) == 0
		&& (*lexer)[(*data).lex_count + 1] == NULL)
		close((*data).fd[0]);
	else
		(*data).input_fd = (*data).fd[0];
	close((*data).fd[1]);
	waitpid(pid, &status, 0);
	if WIFEXITED(status)
		status = WEXITSTATUS(status);
	(*data).lex_count++;
}

int	print_if_correct(t_lexer ***lexer)
{
	if (((*lexer))[1] == NULL)
	{
		ascii_sort(*(get_env)());
		lexer_free(((*lexer)));
		return (1);
	}
	return (0);
}
