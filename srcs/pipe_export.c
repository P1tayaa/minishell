/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 23:13:48 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 23:20:09 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	export_middle_commands(t_lexer ***lexer,
		t_pipedata *data, char **cmd_path, char **args)
{
	(*cmd_path) = get_cmd_path((*lexer)[(*data).lex_count]->cmd, &(*data));
	dup2((*data).input_fd, STDIN_FILENO);
	dup2((*data).fd[1], STDOUT_FILENO);
	close((*data).input_fd);
	close((*data).fd[0]);
	close((*data).fd[1]);
	execve((*cmd_path), args, (*((*data)).environ));
	exit(EXIT_SUCCESS);
}

void	read_writes_export(t_lexer ***lexer,
		t_pipedata *data, char **args, char **cmd_path)
{
	int	count;

	count = 0;
	while ((*lexer)[count] != NULL)
		count++;
	if ((*data).lex_count < count - 1 && (*data).lex_count != 0)
		export_middle_commands(lexer, data, cmd_path, args);
	else if (((*data).lex_count == count - 1)
		&& (*lexer)[(*data).lex_count]->tokenid[0] != '>')
	{
		(*cmd_path) = get_cmd_path((*lexer)[(*data).lex_count]->cmd, &(*data));
		dup2((*data).input_fd, STDIN_FILENO);
		close((*data).input_fd);
		close((*data).fd[0]);
		close((*data).fd[1]);
		execve((*cmd_path), args, (*((*data)).environ));
	}
}

void	out_file_export(t_lexer ***lexer,
		t_pipedata *data, char **cmd_path, char **args)
{
	(*cmd_path) = get_cmd_path((*lexer)[(*data).lex_count]->cmd, &(*data));
	(*data).fd[1] = redirection_handler((*lexer)[(*data).lex_count]);
	dup2((*data).fd[1], STDOUT_FILENO);
	dup2((*data).input_fd, STDIN_FILENO);
	close((*data).fd[0]);
	close((*data).fd[1]);
	close((*data).input_fd);
	execve((*cmd_path), args, (*((*data)).environ));
	exit(EXIT_SUCCESS);
}

void	create_export_child(t_lexer ***lexer,
		t_pipedata *data, char **cmd_path, char **args)
{
	int	pid;

	pid = 0;
	pipe((*data).fd);
	pid = fork();
	if (pid == 0)
	{
		if ((*data).lex_count == 0)
		{
			dup2((*data).fd[1], STDOUT_FILENO);
			close((*data).fd[1]);
			close((*data).fd[0]);
			ascii_sort(*(get_env()));
			exit(EXIT_SUCCESS);
		}
		if ((*lexer)[(*data).lex_count]->tokenid[0] == '>')
			out_file_export(lexer, data, cmd_path, args);
		else
			read_writes_export(lexer, data, args, cmd_path);
	}
	else
		export_parent_management(lexer, data, pid);
}

void	pipe_export(t_lexer ***lexer)
{
	char		*cmd_path;
	char		**args;
	t_pipedata	data;

	initialize_pipedata(&data);
	data.lexer = (*lexer);
	if (pipe_export_error(lexer) == 1)
		return ;
	if ((*lexer)[0]->args == NULL)
	{
		if (print_if_correct(lexer) == 1)
			return ;
		else
		{
			while ((*lexer)[data.lex_count] != NULL)
			{
				if ((*lexer)[data.lex_count]->cmd == NULL)
					break ;
				args = mk_args_for_proc(&data);
				create_export_child(lexer, &data, &cmd_path, args);
				export_content_freeur(&args);
			}
		}
	}
	return ;
}
