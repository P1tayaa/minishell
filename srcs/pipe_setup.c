/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_setup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 23:18:02 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 23:26:20 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_mk_args(t_pipedata *data, char ***args_temp);
int		manage_heredoc(t_pipedata *data, t_lexer **lexer);
void	manage_redirection(t_pipedata *data, t_lexer **lexer, int i);
void	manage_general_outs(t_pipedata *data, t_lexer **lexer, int *out_files);
void	manage_non_redirections(t_pipedata *data, t_lexer **lexer);
void	close_fds(t_pipedata *data);

char	**mk_args_for_proc(t_pipedata *data)
{
	char	**args;
	char	**args_temp;
	int		i;
	int		j;

	i = 0;
	if (data->lexer[data->lex_count]->args != NULL)
	{
		args_temp = split_spaces(data->lexer[data->lex_count]->args);
		while (args_temp[i] != NULL)
			i++;
	}
	args = malloc(sizeof(char *) * (i + 3));
	i = 0;
	args[i++] = ft_strdup(data->lexer[data->lex_count]->cmd);
	if (data->lexer[data->lex_count]->flags != NULL)
		args[i++] = data->lexer[data->lex_count]->flags;
	j = 0;
	if (data->lexer[data->lex_count]->args != NULL)
	{
		while (args_temp[j] != NULL)
			args[i++] = args_temp[j++];
	}
	args[i] = NULL;
	return (free_mk_args(data, &args_temp), args);
}

void	exit_statement(t_pipedata *data)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd((*data).lexer[(*data).lex_count]->cmd, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	exit(127);
}

bool	execute_child_process(t_pipedata *data)
{
	char	*cmd_path;
	char	**args;

	cmd_path = NULL;
	if (data->lexer[data->lex_count]->cmd[0] != '.')
		cmd_path = get_cmd_path(data->lexer[data->lex_count]->cmd, data);
	if (cmd_path == NULL)
	{
		if (access(data->lexer[data->lex_count]->cmd, X_OK) != 0)
		{
			cmd_path = data->lexer[data->lex_count]->cmd;
			exit_statement(data);
		}
	}
	args = mk_args_for_proc(data);
	if (execve(cmd_path, args, (*(*data).environ)) == -1)
	{
		if (cmd_path)
			free(cmd_path);
		export_content_freeur(&args);
		exit (126);
	}
	free(cmd_path);
	export_content_freeur(&args);
	return (1);
}

int	manage_reads_writes(t_pipedata *data, t_lexer **lexer)
{
	int		i;
	int		out_files;

	i = (*data).lex_count;
	if ((*data).input_fd != -1)
		dup2((*data).input_fd, STDIN_FILENO);
	if (lexer[(*data).lex_count]->tokenid[0] == '<')
	{
		if (lexer[(*data).lex_count]->tokenid[1] == '<')
			manage_heredoc(data, lexer);
		else
		{
			manage_redirection(data, lexer, i);
			if (lexer[(*data).lex_count]->cmd == NULL
				&& lexer[(*data).lex_count + 1]->tokenid[0] != '<')
				(*data).lex_count++;
		}
	}
	else if (lexer[(*data).lex_count]->tokenid[0] == '>')
		manage_general_outs(data, lexer, &out_files);
	else
		manage_non_redirections(data, lexer);
	return (close_fds(data), i);
}

int	parent_management(t_pipedata *data, t_lexer **lexer, int pid)
{
	int	status;
	int	store;

	store = -1;
	status = 0;
	if (are_there_more_cmds(lexer, (*data).lex_count) == 0
		&& lexer[(*data).lex_count]->tokenid[0] == '<'
		&& lexer[(*data).lex_count]->tokenid[0] == '>')
		close((*data).fd[0]);
	else
		(*data).input_fd = (*data).fd[0];
	close((*data).fd[1]);
	waitpid(pid, &status, 0);
	if (status == 131 || status == 3)
	{
		ft_putstr_fd("Quit (core dumped)\n", 2);
		store = 131;
	}
	else if WIFEXITED(status)
		status = WEXITSTATUS(status);
	while (wait(NULL) != -1)
		;
	if (store != -1)
		status = store;
	return (manage_signals(0), status);
}
