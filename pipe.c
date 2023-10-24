/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 15:16:04 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/10/24 18:45:33 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	are_there_more_cmds(t_lexer **lexer, int current);

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

char	*get_cmd_path(const char *cmd, t_pipedata *data)
{
	char	*path;
	char	*tmp_path;
	char	*token;
	char	full_path[1024];
	int		i;

	path = NULL;
	i = 0;
	while ((*(*data).environ)[i] != NULL)
	{
		if (strncmp((*(*data).environ)[i], "PATH=", 5) == 0)
		{
			path = (*(*data).environ)[i] + 5;
			break ;
		}
		i++;
	}
	if (!path)
	{
		write(2, "PATH not found in environment\n", 30);
		return (NULL);
	}
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
	return (NULL);
}


int redirection_handler(t_lexer *lexer)
{
	int	fd;

	fd = -1;
	if (ft_memcmp(lexer->tokenid, "<", 1) == 0 && ft_memcmp(lexer->tokenid, "<<", 2) != 0)
		fd = open(lexer->file, O_RDONLY);
	else if (ft_memcmp(lexer->tokenid, ">", 1) == 0)
		fd = open(lexer->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (ft_memcmp(lexer->tokenid, ">>", 2) == 0)
		fd = open(lexer->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (ft_memcmp(lexer->tokenid, "<<", 2) == 0)
		fd = open(lexer->args, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell: ");
		exit(EXIT_FAILURE);
	}
	return (fd);
}

bool	execute_child_process(t_pipedata *data)
{
	char	*cmd_path;
	char	*args[4];
	int		i = 0;

	cmd_path = get_cmd_path(data->lexer[data->lex_count]->cmd, data);
	args[i++] = data->lexer[data->lex_count]->cmd;
	if (data->lexer[data->lex_count]->flags != NULL)
		args[i++] = data->lexer[data->lex_count]->flags;
	if (data->lexer[data->lex_count]->args != NULL)
		args[i++] = data->lexer[data->lex_count]->args;
	args[i] = NULL;
	if (execve(cmd_path, args, (*(*data).environ)) == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd((*data).lexer[(*data).lex_count]->cmd, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		free(cmd_path);
		return (0);	
	}
	free(cmd_path);
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

int	manage_reads_writes(t_pipedata *data, t_lexer **lexer)
{
	int	i;
	char	*str;

	i = (*data).lex_count;
	if ((*data).input_fd != -1)
		dup2((*data).input_fd, STDIN_FILENO);
	if (lexer[(*data).lex_count]->tokenid[0] == '<')
	{
		if (lexer[(*data).lex_count]->tokenid[1] == '<')
		{
			// write(redirection_handler(*lexer), str, ft_strlen(str));
			(*data).fd[1] = redirection_handler(lexer[(*data).lex_count]);
			str = here_doc_starter(lexer[(*data).lex_count]->args);						// eeeeeh it couldn't be that easy
			write((*data).fd[1], str, ft_strlen(str));
			if (lexer[(*data).lex_count + 1]->cmd != NULL)
			{
				puts("this should not print");
				dup2((*data).fd[1], STDOUT_FILENO);
			}
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
		while (lexer[i]->tokenid[0] == '<')
			i++;
		if (i != (*data).lex_count)
			i--;
		(*data).fd[0] = redirection_handler(lexer[i]);
		dup2((*data).fd[0], STDIN_FILENO);
		if (are_there_more_cmds(lexer, (*data).lex_count) != 0)
			dup2((*data).fd[1], STDOUT_FILENO);
		if (lexer[(*data).lex_count]->cmd == NULL && lexer[(*data).lex_count + 1]->tokenid[0] != '<')
			(*data).lex_count++;
	}
	else if (lexer[(*data).lex_count]->tokenid[0] == '>')
	{
		if (lexer[0]->tokenid[0] == '<')
		{
			(*data).fd[1] = redirection_handler(lexer[(*data).lex_count]);
			dup2((*data).fd[1], STDOUT_FILENO);
		}
		else
		{
			(*data).fd[1] = redirection_handler(lexer[(*data).lex_count]);
			dup2((*data).fd[1], STDOUT_FILENO);
			dup2((*data).input_fd, STDIN_FILENO);
		}
	}
	else if ((*data).lex_count != 0 && lexer[(*data).lex_count + 1] != NULL)
	{
		dup2((*data).input_fd, STDIN_FILENO);
		dup2((*data).fd[1], STDOUT_FILENO);
	}
	else if ((*data).lex_count != 0 || (*data).input_fd != -1)			// added top condition here, seems to not break anything? 
		dup2((*data).input_fd, STDIN_FILENO);
	else
		dup2((*data).fd[1], STDOUT_FILENO);
	close((*data).input_fd);
	close((*data).fd[0]);
	close((*data).fd[1]);
	return (i);
}

int	parent_management(t_pipedata *data, t_lexer **lexer, int pid)
{
	int	status;

	status = 0;
	if (are_there_more_cmds(lexer, (*data).lex_count) == 0 && lexer[(*data).lex_count]->tokenid[0] == '<' && lexer[(*data).lex_count]->tokenid[0] == '>')
		close((*data).fd[0]);
	else
		(*data).input_fd = (*data).fd[0];
	close((*data).fd[1]); // Close writing end as we are reading from the pipe
	waitpid(pid, &status, 0);
	if WIFEXITED(status)
		status = WEXITSTATUS(status);
	if WIFSIGNALED(status)
		status = WTERMSIG(status);
	if WIFSTOPPED(status)
		status = WSTOPSIG(status);
	return (status);
}

int	is_built_in(t_lexer **lexer, int lex_count)
{
	int			i;
	int			cmd_len;
	int			builtin_len;
	const char* builtins[] = {
        "pwd",
        "echo",
        "exit",
		"clear",
		"cd",
		"env",
		"^D",
        NULL
    };

	i = 0;
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

int	are_all_commands_thesame(t_lexer **lexer)
{
	int	collapse;
	int	i;

	i = 0;
	collapse = 0;
	while (lexer[i] != NULL)
		i++;
	while (lexer[collapse] != NULL)
	{
		if (lexer[collapse + 1] != NULL)
		{
			if (ft_memcmp(lexer[collapse]->cmd, lexer[collapse + 1]->cmd, ft_strlen(lexer[collapse]->cmd)) == 0)
				collapse++;
			else
				break ;
		}
		else
			break ;
	}
	printf("collapse = %i\ni = %i\n", collapse, i);
	if (collapse + 1 == i)
		return (1);
	else
		return (0);  
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
	if (lexer[data.lex_count]->cmd != NULL && lexer[data.lex_count]->execd == false && lexer[data.lex_count]->tokenid[0] != '<')																		// collapse
	{
		if (lexer[data.lex_count + 1] != NULL)
		{
			if (are_all_commands_thesame(lexer) == 1)
			{
				lexer[data.lex_count]->args = replace_doll_question_to_number_with_free(lexer[data.lex_count]->args, doll);
				pid = fork();
				if (pid == 0)
				{
					execute_child_process(&data);
					exit(EXIT_SUCCESS);
				}
				lexer[data.lex_count]->execd = true;
				waitpid(pid, &doll, 0);
				if WIFEXITED(doll)
					doll = WEXITSTATUS(doll);
				else if WIFSIGNALED(doll)
					doll = WTERMSIG(doll);
				else if WIFSTOPPED(doll)
					doll = WSTOPSIG(doll);
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
				manage_reads_writes(&data, lexer);		// info =
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
