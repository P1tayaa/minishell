/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 15:16:04 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/10/13 14:15:46 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char	**environ;

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

char	*get_cmd_path(const char *cmd)
{
	char	*path;
	char	*tmp_path;
	char	*token;
	char	full_path[1024];
	int		i;

	path = NULL;
	i = 0;
	while (environ[i] != NULL)
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
		{
			path = environ[i] + 5;
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
	if (strcmp(lexer->tokenid, "<") == 0)
		fd = open(lexer->file, O_RDONLY);
	else if (strcmp(lexer->tokenid, ">") == 0)
		fd = open(lexer->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (strcmp(lexer->tokenid, ">>") == 0)
		fd = open(lexer->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("Error in redirection_handler");
		exit(EXIT_FAILURE);
	}
	return (fd);
}

void	execute_child_process(t_pipedata *data)
{
	char	*cmd_path;
	char	*args[4];
	int		i = 0;

	cmd_path = get_cmd_path(data->lexer[data->lex_count]->cmd);
	args[i++] = data->lexer[data->lex_count]->cmd;
	if (data->lexer[data->lex_count]->flags != NULL)
		args[i++] = data->lexer[data->lex_count]->flags;
	if (data->lexer[data->lex_count]->args != NULL)
		args[i++] = data->lexer[data->lex_count]->args;
	args[i] = NULL;
	execve(cmd_path, args, environ);
	free(cmd_path);
	perror("Execution failed");
	exit(EXIT_FAILURE);
}

void	initialize_pipedata(t_pipedata *data)
{
	data->input_fd = -1;
	data->lex_count = 0;
}

void	manage_reads_writes(t_pipedata *data, t_lexer **lexer)
{
	// if ((*data).input_fd != -1)
	// {
	// 	dup2((*data).input_fd, STDIN_FILENO);
	// 	close((*data).input_fd);
	// }
	if (is_built_in(lexer, data) == 1)
	{
		puts("i am in isbuilt");
		dup2((*data).fd[1], STDOUT_FILENO);
		puts("test");
		close((*data).fd[1]);
		puts("test");
		close((*data).fd[0]);
		puts("test");
		return ;
	}
	if (lexer[(*data).lex_count]->tokenid[0] == '<')
	{
		(*data).fd[0] = redirection_handler(lexer[(*data).lex_count]);
		dup2((*data).fd[0], STDIN_FILENO);
		dup2((*data).fd[1], STDOUT_FILENO);
		(*data).lex_count++;
	}
	else if (lexer[(*data).lex_count]->tokenid[0] == '>')
	{
		(*data).fd[1] = redirection_handler(lexer[(*data).lex_count]);
		dup2((*data).fd[1], STDOUT_FILENO);
		dup2((*data).input_fd, STDIN_FILENO);
		close((*data).input_fd);
	}
	else if ((*data).lex_count != 0 && lexer[(*data).lex_count + 1] != NULL)
	{
		dup2((*data).input_fd, STDIN_FILENO);
		close((*data).input_fd);
		dup2((*data).fd[1], STDOUT_FILENO);
	}
	else if ((*data).lex_count != 0 || (*data).input_fd != -1)			// added top condition here, seems to not break anything? 
	{
		puts("I am in here");
		dup2((*data).input_fd, STDIN_FILENO);
		close((*data).input_fd);
	}
	else
	{
		puts("in else");
		dup2((*data).fd[1], STDOUT_FILENO);
	}
	puts("finished managing reads/writes");
	close((*data).fd[0]);
	close((*data).fd[1]);
}

void	parent_management(t_pipedata *data, t_lexer **lexer, int pid)
{
	int	status;

	status = 0;
	close((*data).fd[1]); // Close writing end as we are reading from the pipe
	if ((lexer[(*data).lex_count + 1] == NULL || lexer[(*data).lex_count + 1]->cmd == NULL) && lexer[(*data).lex_count]->tokenid[0] == '<')
		close((*data).fd[0]);
	else
	{
		puts("set input_fd to fd[0]");
		(*data).input_fd = (*data).fd[0];
	}
	waitpid(pid, &status, 0);
}

int	is_built_in(t_lexer **lexer, t_pipedata *data)
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
	puts("checking builtins");
	while (builtins[i])
	{
		cmd_len = ft_strlen(lexer[(*data).lex_count]->cmd);
		builtin_len = ft_strlen(builtins[i]);
		if (cmd_len > builtin_len)
		{
			if (ft_strncmp(lexer[(*data).lex_count]->cmd, builtins[i], builtin_len) == 0)
				return (1);
		}
		else if (ft_strncmp(lexer[(*data).lex_count]->cmd, builtins[i], cmd_len) == 0)
			return (1);
		i++;
	}
	return (0);
}

void	piping(t_lexer **lexer)
{
	t_pipedata	data;
	int			pid;

	initialize_pipedata(&data);
	data.lexer = lexer;
	puts("pipes");
	if (lexer[data.lex_count]->tokenid[0] != '<' && is_built_in(lexer, &data) == 1)
	{
		puts("built in");
		if (lexer[1] == NULL)
		{
			puts("single builtin");
			executer(lexer, &data);
			lexer[data.lex_count]->execd = true;
		}
	}
	if (lexer[1] == NULL && lexer[data.lex_count]->execd == false)
	{
		puts("single exec");
		if ((pid = fork()) == 0)
		{
			execute_child_process(&data);
			exit(EXIT_SUCCESS);
		}
		// lexer[data.lex_count]->execd = true;
	}
	while (lexer[data.lex_count] != NULL && lexer[data.lex_count]->execd == false)
	{
		puts("piping");
		pipe(data.fd);
		pid = fork();
		if (pid == -1)
		{
			perror("Fork failed");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			puts("in child process");
			manage_reads_writes(&data, lexer);
			if (lexer[data.lex_count]->cmd)
			{
				if (is_built_in(lexer, &data) == 1)
				{
					puts("found built in");
					executer(lexer, &data);
				}
				else
					execute_child_process(&data);
			}
		}
		else  // Parent process
			parent_management(&data, lexer, pid);
		// puts("incrementing lex_count");
		data.lex_count++;
	}
}



		// else
		// {
		// 	puts("hardcoded piping builtins");
		// 	pipe(data.fd);
		// 	pid = fork();
		// 	if (pid == 0)
		// 	{
		// 		dup2(data.fd[1], STDOUT_FILENO);
		// 		close(data.fd[1]);
		// 		close(data.fd[0]);
		// 		executer(lexer, &data);
		// 		exit(EXIT_SUCCESS);
		// 	}
		// 	lexer[data.lex_count]->execd = true;
		// 	data.lex_count++;
		// 	pid = fork();
		// 	if (pid == 0)
		// 	{
		// 		puts("second process");
		// 		dup2(data.fd[0], STDIN_FILENO);
		// 		close(data.fd[1]);
		// 		execute_child_process(&data);
		// 		exit(EXIT_SUCCESS);
		// 	}
		// 	lexer[data.lex_count]->execd = true;
		// 	close(data.fd[0]);
		// 	close(data.fd[1]);
		// 	wait(NULL);
		// 	wait(NULL);
		// }








// void	exec(t_lexer *lexer)
// {
// 	char	*args[4];  // Assuming a maximum of one flag and one argument for simplicity.
// 	char	*cmd_path;
// 	int		i;
// 	pid_t	pid;

// 	cmd_path = get_cmd_path(lexer->cmd);
// 	if (cmd_path == NULL)
// 	{
// 		perror("Command not found EXEC");
// 		return ;
// 	}
// 	i = 0;
// 	args[i++] = lexer->cmd;
// 	if (lexer->flags != NULL)
// 		args[i++] = lexer->flags;
// 	if (lexer->args != NULL)
// 		args[i++] = lexer->args;
// 	args[i++] = NULL;
// 	i = 0;
// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		perror("Fork failed");
// 		free(cmd_path);
// 		return ;
// 	}
// 	if (pid == 0)				// Child process
// 	{
// 		execve(cmd_path, args, environ);
// 		perror("Execution failed");
// 		exit(EXIT_FAILURE);
// 	}
// 	else						// Parent process
// 		wait(NULL);				// Wait for the child to finish
// 	free(cmd_path);
// }