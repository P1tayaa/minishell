/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 15:16:04 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/21 15:35:08 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		are_there_more_cmds(t_lexer **lexer, int current);
char	*sjoin_fr(char *s1, char *s2);
char	***get_env(void);
void	ascii_sort(char **environment);
bool	is_str_export(char *str);
void	lexer_free(t_lexer **lexer);
void	restore_default_sigint_handling(void);
int		manage_reads_writes(t_pipedata *data, t_lexer **lexer);
char	*doll_to_num(char *str_og, int number_replace);

int	open_file(char *filename, int mode)
{
	if (mode == 0)
	{
		if (access(filename, F_OK) < 0)
		{
			write(2, "No such file or directory\n", 26);
			return (0);
		}
		return (open(filename, O_RDONLY));
	}
	else
	{
		return (open(filename, O_CREAT | O_WRONLY | O_TRUNC,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH));
	}
}

char	*check_path(t_pipedata *data)
{
	char	*path;
	int		i;

	path = NULL;
	i = 0;
	while ((*data->environ)[i] != NULL)
	{
		if (ft_memcmp((*data->environ)[i], "PATH=", 5) == 0)
		{
			path = (*data->environ)[i] + 5;
			break ;
		}
		i++;
	}
	if (!path)
	{
		write(2, "PATH not found in environment\n", 30);
		return (NULL);
	}
	return (ft_strdup(path));
}

char	*get_cmd_path(const char *cmd, t_pipedata *data)
{
	char	*path;
	char	*tmp_path;
	char	*token;
	char	full_path[1024];

	path = check_path(data);
	tmp_path = strdup(path);
	token = ft_strtok(tmp_path, ":");
	while (token)
	{
		concat_path(full_path, token, cmd);
		if (access(full_path, X_OK) == 0)
		{
			free(tmp_path);
			return (strdup(full_path));
		}
		free(token);
		token = ft_strtok(NULL, ":");
	}
	free(token);
	free(tmp_path);
	free(path);
	return (NULL);
}

int	redirection_handler(t_lexer *lexer)
{
	int	fd;

	fd = -1;
	if (ft_memcmp(lexer->tokenid, "<", 1) == 0
		&& ft_memcmp(lexer->tokenid, "<<", 2) != 0)
		fd = open(lexer->file, O_RDONLY);
	else if (ft_memcmp(lexer->tokenid, ">", 1) == 0
		&& ft_memcmp(lexer->tokenid, ">>", 2) != 0)
		fd = open(lexer->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (ft_memcmp(lexer->tokenid, ">>", 2) == 0)
	{
		if (access(lexer->file, F_OK) != -1)
			fd = open(lexer->file, O_WRONLY | O_APPEND, 0644);
		else
			fd = open(lexer->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	else if (ft_memcmp(lexer->tokenid, "<<", 2) == 0)
		fd = open(lexer->args, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell: ");
		exit(EXIT_FAILURE);
	}
	return (fd);
}

char	**split_spaces(char *str);
void	export_content_freeur(char ***export_content);

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
	args[i++] = data->lexer[data->lex_count]->cmd;
	if (data->lexer[data->lex_count]->flags != NULL)
		args[i++] = data->lexer[data->lex_count]->flags;
	j = 0;
	if (data->lexer[data->lex_count]->args != NULL)
		while (args_temp[j] != NULL)
			args[i++] = args_temp[j++];
	args[i] = NULL;
	if (data->lexer[data->lex_count]->args != NULL)
		free(args_temp);
	return (args);
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

int	manage_heredoc(t_pipedata *data, t_lexer **lexer)
{
	char	*str;

	str = NULL;
	if (lexer[(*data).lex_count]->tokenid[1] == '<')
	{
		str = here_doc_starter(lexer[(*data).lex_count]->args);
		if (!str)
			return (-1);
		lexer[(*data).lex_count]->file = sjoin_fr(
				lexer[(*data).lex_count]->file, ft_strdup("heredoc"));
		(*data).fd[1] = redirection_handler(lexer[(*data).lex_count]);
		write((*data).fd[1], str, ft_strlen(str));
		if (lexer[(*data).lex_count + 1]->cmd != NULL)
			dup2((*data).fd[1], STDOUT_FILENO);
		free(str);
		close((*data).input_fd);
		close((*data).fd[0]);
		close((*data).fd[1]);
		(*data).lex_count = are_there_more_cmds(lexer, (*data).lex_count);
		if ((*data).lex_count == 0)
			return ((*data).lex_count);
		else
			return (manage_reads_writes(data, lexer));
	}
	return (0);
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

int	manage_reads_writes(t_pipedata *data, t_lexer **lexer)
{
	int		i;
	int		out_files;

	i = (*data).lex_count;
	if ((*data).input_fd != -1)
		dup2((*data).input_fd, STDIN_FILENO);
	if (lexer[(*data).lex_count]->tokenid[0] == '<')
	{
		manage_heredoc(data, lexer);
		manage_redirection(data, lexer, i);
		if (lexer[(*data).lex_count]->cmd == NULL
			&& lexer[(*data).lex_count + 1]->tokenid[0] != '<')
			(*data).lex_count++;
	}
	else if (lexer[(*data).lex_count]->tokenid[0] == '>')
	{
		manage_multiple_outs(data, lexer, &out_files);
		manage_outwrite(lexer, data, &out_files);
	}
	else
		manage_non_redirections(data, lexer);
	close((*data).input_fd);
	close((*data).fd[0]);
	close((*data).fd[1]);
	return (i);
}

int	parent_management(t_pipedata *data, t_lexer **lexer, int pid)
{
	int	status;

	status = 0;
	if (are_there_more_cmds(lexer, (*data).lex_count) == 0
		&& lexer[(*data).lex_count]->tokenid[0] == '<'
		&& lexer[(*data).lex_count]->tokenid[0] == '>')
		close((*data).fd[0]);
	else
		(*data).input_fd = (*data).fd[0];
	close((*data).fd[1]);
	waitpid(pid, &status, 0);
	if WIFEXITED(status)
		status = WEXITSTATUS(status);
	if WIFSIGNALED(status)
		status = WEXITSTATUS(status);
	if WIFSTOPPED(status)
		status = WEXITSTATUS(status);
	return (status);
}

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
					restore_default_sigint_handling();
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
			execute_child_process(data);
			exit(EXIT_SUCCESS);
		}
		waitpid((*pid), &(*doll), 0);
		if WIFEXITED((*doll))
			(*doll) = WEXITSTATUS((*doll));
	}
}

void	create_and_run_child(t_pipedata *data,
		t_lexer **lexer, int *pid, int *doll)
{
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
		restore_default_sigint_handling();
		if (manage_reads_writes(&(*data), lexer) == -1)
			return ;
		if (lexer[(*data).lex_count]->cmd)
			execute_child_process(&(*data));
	}
	else
		(*doll) = parent_management(&(*data), lexer, (*pid));
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

void	piping(t_lexer **lexer)
{
	t_pipedata			data;
	int					pid;
	static int			doll;

	initialize_pipedata(&data);
	if (lexer[0] == NULL)
		return ;
	data.lexer = lexer;
	manage_collapse(lexer, &data, &pid, doll);
	manage_single_execs(lexer, &data, &pid, &doll);
	pipes(lexer, &data, &pid, &doll);
	if (lexer[data.lex_count]->tokenid[0] == '<')
	{
		if (lexer[data.lex_count]->tokenid[1] == '<')
			unlink(lexer[data.lex_count]->args);
	}
	dup2(data.og_out, STDOUT_FILENO);
	dup2(data.og_in, STDIN_FILENO);
	close(data.og_out);
	close(data.og_in);
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
		restore_default_sigint_handling();
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
				if (is_str_export((*lexer)[data.lex_count]->cmd) == false)
					args = mk_args_for_proc(&data);
				create_export_child(lexer, &data, &cmd_path, args);
			}
		}
	}
	return ;
}
