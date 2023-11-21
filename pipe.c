/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 15:16:04 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/20 22:06:56 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		are_there_more_cmds(t_lexer **lexer, int current);
char	*sjoin_fr(char *s1, char *s2);
char	***get_env(void);
void	ascii_sort(char **environment);
bool	is_str_export(char *str);
void	lexer_free(t_lexer **lexer);
void restore_default_sigint_handling();

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
	char *path;
	int i;

	path = NULL;
	i = 0;
	while ((*data->environ)[i] != NULL)
	{
		if (ft_memcmp((*data->environ)[i], "PATH=", 5) == 0)
		{
			path = (*data->environ)[i] + 5;
			break;
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
	// int		i;

	path = check_path(data);
	tmp_path = strdup(path);
	token = ft_strtok(tmp_path, ":");
	while (token)
	{
		// build_path(full_path, token, cmd, tmp)
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


int redirection_handler(t_lexer *lexer)
{
	int	fd;

	fd = -1;
	if (ft_memcmp(lexer->tokenid, "<", 1) == 0 && ft_memcmp(lexer->tokenid, "<<", 2) != 0)
		fd = open(lexer->file, O_RDONLY);
	else if (ft_memcmp(lexer->tokenid, ">", 1) == 0 && ft_memcmp(lexer->tokenid, ">>", 2) != 0)
	{
		puts("single > redirect");
		fd = open(lexer->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
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

char    **ft_split_world_at_all_spaces(char *str);
void    export_content_freeur(char ***export_content);

char **mk_args_for_proc(t_pipedata *data)
{
    char	**args;
    char	**args_temp;
    int		i;
	int		j;

    i = 0;
    if (data->lexer[data->lex_count]->args != NULL)
    {
        args_temp = ft_split_world_at_all_spaces(data->lexer[data->lex_count]->args);
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

bool	execute_child_process(t_pipedata *data)
{
	char	*cmd_path;
	char	**args;

	cmd_path = NULL;
	if (data->lexer[data->lex_count]->cmd[0] != '.')
		cmd_path = get_cmd_path(data->lexer[data->lex_count]->cmd, data);
	if (cmd_path == NULL)
	{
		puts("I WANT TO EXIT 127");
		if (access(data->lexer[data->lex_count]->cmd, X_OK) != 0)
		{
			cmd_path = data->lexer[data->lex_count]->cmd;
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd((*data).lexer[(*data).lex_count]->cmd, STDERR_FILENO);
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
			exit(127);
		}
	}
	args = mk_args_for_proc(data);
	if (execve(cmd_path, args, (*(*data).environ)) == -1)
	{
		if (cmd_path)
			free(cmd_path);
		puts("I WANT TO EXIT 126");
		export_content_freeur(&args);
		exit (126);
	}
	free(cmd_path);
	export_content_freeur(&args);
	return (1);
}

void	initialize_pipedata(t_pipedata *data)
{
	extern char **environ;

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

int	manage_reads_writes(t_pipedata *data, t_lexer **lexer)
{
	int	i;
	int	out_files;
	int	temp_fd;
	char	*str;

	i = (*data).lex_count;
	temp_fd = 0;
	if ((*data).input_fd != -1)
		dup2((*data).input_fd, STDIN_FILENO);
	if (lexer[(*data).lex_count]->tokenid[0] == '<')
	{
		if (lexer[(*data).lex_count]->tokenid[1] == '<')
		{
			str = here_doc_starter(lexer[(*data).lex_count]->args);
			if (!str)
				return (-1);
			lexer[(*data).lex_count]->file = sjoin_fr(lexer[(*data).lex_count]->file, ft_strdup("heredoc"));
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
		(*data).fd[0] = redirection_handler(lexer[i]);
		dup2((*data).fd[0], STDIN_FILENO);
		printf("are there more commands? = %i\nAre there previous commands? = %i\n", are_there_more_cmds(lexer, (*data).lex_count), are_there_previous_cmds(lexer, data));
		if (are_there_more_cmds(lexer, (*data).lex_count) != 0)		// are_there_previous_cmds(lexer, data) != -1
		{
			puts("im dumb");
			dup2((*data).fd[1], STDOUT_FILENO);
		}
		if (lexer[(*data).lex_count + 1] != NULL)			// !! was needed for grep NAME < Makefile > test.txt scenario !!
		{
			if (lexer[(*data).lex_count + 1]->tokenid[0] == '>')
			{
				if (lexer[(*data).lex_count + 2] != NULL)
				{
					if (lexer[(*data).lex_count + 2]->tokenid[0] != '>')
					{
						(*data).fd[1] = redirection_handler(lexer[(*data).lex_count + 1]);
						dup2((*data).fd[1], STDOUT_FILENO);
					}
				}
				else
				{
					(*data).fd[1] = redirection_handler(lexer[(*data).lex_count + 1]);
					dup2((*data).fd[1], STDOUT_FILENO);
				}
			}
		}
		if (lexer[(*data).lex_count]->cmd == NULL && lexer[(*data).lex_count + 1]->tokenid[0] != '<')
			(*data).lex_count++;
	}
	else if (lexer[(*data).lex_count]->tokenid[0] == '>')
	{
		puts("walk in here");
		out_files = (*data).lex_count;
		while (lexer[out_files] != NULL)
		{
			if (lexer[out_files + 1] != NULL)
			{
				if (lexer[out_files + 1]->tokenid[0] == '>')
				{
					printf("out_files = %i\n", out_files);
					temp_fd = open(lexer[out_files]->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
					close(temp_fd);
					out_files++;
				}
				else
					break ;
			}
		}
		if (out_files != 0)
		{
			if (lexer[out_files - 1]->tokenid[0] == '>')
			{
				printf("out_file = %i\n", out_files);
				printf("file = %s\n", lexer[out_files]->file);
				(*data).fd[1] = redirection_handler(lexer[out_files]);
				dup2((*data).fd[1], STDOUT_FILENO);
			}
		}
		if (lexer[0]->tokenid[0] == '<')		// needed if pipe first then redirect, but breaks for no pipe into redirect: e.g. < Makefile grep NAME > test.txt
		{
			puts("pls dont");
			if (lexer[1] != NULL)
			{
				if (ft_memcmp(lexer[1]->tokenid, "|", 1) == 0)
				{
					puts("nei");
					(*data).fd[1] = redirection_handler(lexer[(*data).lex_count]);
					dup2((*data).fd[1], STDOUT_FILENO);

				}
			}
		}
		else if (out_files == (*data).lex_count)
		{
			puts("gey");
			(*data).fd[1] = redirection_handler(lexer[(*data).lex_count]);
			dup2((*data).fd[1], STDOUT_FILENO);
			dup2((*data).input_fd, STDIN_FILENO);
		}
	}
	else if ((*data).lex_count != 0 && lexer[(*data).lex_count + 1] != NULL)
	{
		puts("first else");
		dup2((*data).input_fd, STDIN_FILENO);
		dup2((*data).fd[1], STDOUT_FILENO);
	}
	else if ((*data).lex_count != 0 || (*data).input_fd != -1)			// added top condition here, seems to not break anything?
	{
		puts("second else");
		dup2((*data).input_fd, STDIN_FILENO);
	}
	else
	{
		puts("third else");
		printf("fd[1] = %i\n", (*data).fd[1]);
		dup2((*data).fd[1], STDOUT_FILENO);
	}
	close((*data).input_fd);
	close((*data).fd[0]);
	close((*data).fd[1]);
	return (i);
}

int	parent_management(t_pipedata *data, t_lexer **lexer, int pid)
{
	int	status;

	status = 0;
	if (are_there_more_cmds(lexer, (*data).lex_count) == 0 &&
		lexer[(*data).lex_count]->tokenid[0] == '<' && lexer[(*data).lex_count]->tokenid[0] == '>')
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
			if (ft_strncmp(lexer[lex_count]->cmd, builtins[i], builtin_len) == 0)
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
			if ((len_collapse < len_next && ft_memcmp(lexer[collapse]->cmd, lexer[collapse + 1]->cmd, len_collapse) == 0) ||
				(len_collapse >= len_next && ft_memcmp(lexer[collapse]->cmd, lexer[collapse + 1]->cmd, len_next) == 0))
				collapse++;
			else
				break;
		}
		else
			break;
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

void	weird_signals(int sig)
{
	if (sig == SIGINT)
	{
		puts("HIIIII");
		// rl_on_new_line();
		rl_replace_line("", 0);
		// rl_redisplay();
	}
}

void	empty_cmd_args_signals()
{
	struct sigaction	s1;

	puts("oioiooi");
	s1.sa_handler = &weird_signals;
	sigemptyset(&(s1.sa_mask));
	sigaddset(&s1.sa_mask, SIGINT);
	s1.sa_flags = 0;
	sigaction(SIGINT, &s1, NULL);
	return ;
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
	if (lexer[data.lex_count]->cmd != NULL && lexer[data.lex_count]->execd == false && lexer[data.lex_count]->tokenid[0] != '<' && lexer[data.lex_count]->tokenid[0] != '>')					// collapse
	{
		if (lexer[data.lex_count + 1] != NULL)
		{
			if (are_all_commands_thesame(lexer) == 1)			// bug is here
			{
				lexer[data.lex_count]->args = replace_doll_question_to_number_with_free(lexer[data.lex_count]->args, doll);
				pid = fork();
				if (pid == 0)
				{
					restore_default_sigint_handling();
					execute_child_process(&data);
					exit(EXIT_SUCCESS);
				}
				lexer[data.lex_count]->execd = true;
				waitpid(pid, &doll, 0);
				if WIFEXITED(doll)
					doll = WEXITSTATUS(doll);
			}
		}
	}
	if (lexer[data.lex_count]->tokenid[0] != '<' && is_built_in(lexer, data.lex_count) == 1)					// builtin
	{
		if (lexer[1] == NULL)
		{
			lexer[data.lex_count]->args = replace_doll_question_to_number_with_free(lexer[data.lex_count]->args, doll);
			doll = executer(lexer, &data);
			lexer[data.lex_count]->execd = true;
		}
	}
	if (lexer[1] == NULL && lexer[data.lex_count]->execd == false)												// single exec non-built-in
	{
		if ((pid = fork()) == 0)
		{
			// empty_cmd_args_signals();
			lexer[data.lex_count]->args = replace_doll_question_to_number_with_free(lexer[data.lex_count]->args, doll);
			execute_child_process(&data);
			exit(EXIT_SUCCESS);
		}
		waitpid(pid, &doll, 0);
		if WIFEXITED(doll)
			doll = WEXITSTATUS(doll);
		else if WIFSIGNALED(doll)
			doll = WTERMSIG(doll);
		else if WIFSTOPPED(doll)
			doll = WSTOPSIG(doll);
	}
	if (lexer[1] != NULL)																						// piping/redirections
		while (lexer[data.lex_count] != NULL && lexer[data.lex_count]->execd == false)
		{
			if (lexer[data.lex_count]->cmd != NULL)
				if (ft_memcmp(lexer[data.lex_count]->cmd, "cat", 3) == 0)
				{
					if (lexer[data.lex_count]->args == NULL)
					{
						if (lexer[data.lex_count + 1] != NULL)
						{
							readline("\n");
							data.lex_count++;
							continue ;
						}
					}
				}
			lexer[data.lex_count]->args = replace_doll_question_to_number_with_free(lexer[data.lex_count]->args, doll);
			pipe(data.fd);
			pid = fork();
			if (pid == -1)
			{
				perror("Fork failed");
				exit(EXIT_FAILURE);
			}
			if (pid == 0)
			{
				puts("in child process loop");
				restore_default_sigint_handling();
				if (manage_reads_writes(&data, lexer) == -1)
					return ;
				if (lexer[data.lex_count]->cmd)
				{
					if (is_built_in(lexer, data.lex_count) == 1)
						execute_child_process(&data);
					else
						execute_child_process(&data);
				}
			}
			else  // Parent process
				doll = parent_management(&data, lexer, pid);
			if (lexer[data.lex_count + 1] == NULL || are_there_more_cmds(lexer, data.lex_count) == 0)
				break ;
			else
				data.lex_count = are_there_more_cmds(lexer, data.lex_count);
		}
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
	return(0);
}

bool	pipe_export(t_lexer ***lexer)
{
	int			pid;
	int			status;
	int			i;
	int			count;
	char		*cmd_path;
	char		*args[4];
	t_pipedata	data;

	i = 0;
	pid = 0;
	count = 0;
	initialize_pipedata(&data);
	while ((*lexer)[count] != NULL)
		count++;
	if (((*lexer))[1] != NULL && ((*lexer))[0]->args != NULL)
	{
		write(2, "Can't pipe export when setting value\n", 37);
		lexer_free(((*lexer)));
		return (true);
	}
	if ((*lexer)[0]->args == NULL)
	{
		if (((*lexer))[1] == NULL)
		{
			ascii_sort(*(get_env)());
			lexer_free(((*lexer)));
			return (true);
		}
		else
		{
			while ((*lexer)[data.lex_count] != NULL)
			{
				if ((*lexer)[data.lex_count]->cmd == NULL)
					break ;
				if (is_str_export((*lexer)[data.lex_count]->cmd) == false) 
				{
					args[i++] = (*lexer)[data.lex_count]->cmd;
					if ((*lexer)[data.lex_count]->flags != NULL)
						args[i++] = (*lexer)[data.lex_count]->flags;
					if ((*lexer)[data.lex_count]->args != NULL)
						args[i++] = (*lexer)[data.lex_count]->args;
					args[i] = NULL;
				}
				i = 0;
				pipe(data.fd);
				pid = fork();
				if (pid == 0)
				{
					restore_default_sigint_handling();
					if (data.lex_count == 0)
					{
						dup2(data.fd[1], STDOUT_FILENO);
						close(data.fd[1]);
						close(data.fd[0]);
						ascii_sort(*(get_env()));
						exit(EXIT_SUCCESS);
					}
					if ((*lexer)[data.lex_count]->tokenid[0] == '>')
					{
						cmd_path = get_cmd_path((*lexer)[data.lex_count]->cmd, &data);
						data.fd[1] = redirection_handler((*lexer)[data.lex_count]);
						dup2(data.fd[1], STDOUT_FILENO);
						dup2(data.input_fd, STDIN_FILENO);
						close(data.fd[0]);
						close(data.fd[1]);
						close(data.input_fd);
						execve(cmd_path, args, (*(data).environ));
						exit(EXIT_SUCCESS);
					}
					else if (data.lex_count < count - 1 && data.lex_count != 0)
					{
						cmd_path = get_cmd_path((*lexer)[data.lex_count]->cmd, &data);
						dup2(data.input_fd, STDIN_FILENO);
						dup2(data.fd[1], STDOUT_FILENO);
						close(data.input_fd);
						close(data.fd[0]);
						close(data.fd[1]);
						execve(cmd_path, args, (*(data).environ));
						exit(EXIT_SUCCESS);
					}
					else if ((data.lex_count == count - 1) && (*lexer)[data.lex_count]->tokenid[0] != '>')
					{
						cmd_path = get_cmd_path((*lexer)[data.lex_count]->cmd, &data);
						dup2(data.input_fd, STDIN_FILENO);
						close(data.input_fd);
						close(data.fd[0]);
						close(data.fd[1]);
						execve(cmd_path, args, (*(data).environ));
					}
				}
				else
				{
					if (are_there_more_cmds((*lexer), data.lex_count) == 0 && (*lexer)[data.lex_count + 1] == NULL)
						close(data.fd[0]);
					else
						data.input_fd = data.fd[0];
					close(data.fd[1]);
					waitpid(pid, &status, 0);
					if WIFEXITED(status)
						status = WEXITSTATUS(status);
					data.lex_count++;
				}
			}
		}
		return (true);
	}
	return (false);
}
