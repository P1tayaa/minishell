/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 15:16:04 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/10/07 17:57:36 by oscarmathot      ###   ########.fr       */
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

void	exec(t_lexer *lexer)
{
	char	*args[4];  // Assuming a maximum of one flag and one argument for simplicity.
	char	*cmd_path;
	int		i;
	pid_t	pid;

	cmd_path = get_cmd_path(lexer->cmd);
	if (cmd_path == NULL)
	{
		perror("Command not found EXEC");
		return ;
	}
	i = 0;
	args[i++] = lexer->cmd;
	if (lexer->flags != NULL)
		args[i++] = lexer->flags;
	if (lexer->args != NULL)
		args[i++] = lexer->args;
	args[i++] = NULL;
	i = 0;
	pid = fork();
	if (pid == -1)
	{
		perror("Fork failed");
		free(cmd_path);
		return ;
	}
	if (pid == 0)				// Child process
	{
		execve(cmd_path, args, environ);
		perror("Execution failed");
		exit(EXIT_FAILURE);
	}
	else						// Parent process
		wait(NULL);				// Wait for the child to finish
	free(cmd_path);
}

int redirection_handler(t_lexer *lexer)
{
	int	fd;

	fd = -1;
	// puts("launches redirection_handler");
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

void	set_redirection(t_lexer **lexer, int *input_fd, int j)
{
	if (j > 0 && (strcmp(lexer[j-1]->tokenid, "<") == 0 || strcmp(lexer[j-1]->tokenid, ">") == 0 || strcmp(lexer[j-1]->tokenid, ">>") == 0))
	{
		*input_fd = redirection_handler(lexer[j-1]);
		if (*input_fd == -1)
		{
			perror("Error setting up redirection");
			exit(EXIT_FAILURE);
		}
	}
}

    // If there's an input redirection, use it
    // if (data->input_fd != -1)
    // {
    //     if (dup2(data->input_fd, STDIN_FILENO) == -1) {
    //         perror("dup2 input_fd");
    //         exit(EXIT_FAILURE);
    //     }
    //     close(data->input_fd);
    // }
    // if (data->lex_count != 0)  // If not the first command and no input redirection
    // {
    //     if (dup2(data->input_fd, STDIN_FILENO) == -1) {
    //         perror("dup2 prev_fd");
    //         exit(EXIT_FAILURE);
    //     }
    //     close(data->input_fd);
    // }

    // // If this is the last command and there's an output redirection, use it
	// if (!(data->lexer[data->lex_count + 1]))
	// {
    // 	fprintf(stderr, "Debug: output_fd before dup2: %d\n", data->output_fd);  // Debug print
    // 	if (dup2(data->output_fd, STDOUT_FILENO) == -1) {
    // 	    perror("dup2 output_fd");
    // 	    exit(EXIT_FAILURE);
    // 	}
    // 	close(data->output_fd);
	// }

    // // If not the last command, set up for next command
    // if (data->lexer[data->lex_count + 1]->cmd != NULL)
    // {
    //     if (dup2(data->fd[1], STDOUT_FILENO) == -1) {
    //         perror("dup2 fd[1]");
    //         exit(EXIT_FAILURE);
    //     }
    // }

    // close(data->fd[0]);
    // close(data->fd[1]);

void execute_child_process(t_pipedata *data) 
{
    char *cmd_path;
    char *args[4];
    int i = 0;

	// puts("retrieving cmd_path");
    cmd_path = get_cmd_path(data->lexer[data->lex_count]->cmd);
	// printf("Command path: %s\n", cmd_path);
    args[i++] = data->lexer[data->lex_count]->cmd;
    if (data->lexer[data->lex_count]->flags != NULL)
        args[i++] = data->lexer[data->lex_count]->flags;
    if (data->lexer[data->lex_count]->args != NULL)
	{
        args[i++] = data->lexer[data->lex_count]->args;
		// printf("args: %s\n", args[i - 1]);
	}
    args[i] = NULL;  // Ensure the args array is NULL-terminated
    execve(cmd_path, args, environ);
    free(cmd_path);
    // If execve returns, it means there was an error
    perror("Execution failed");
    exit(EXIT_FAILURE);
}


void	initialize_pipedata(t_pipedata *data)
{
	data->input_fd = -1;
	data->lex_count = 0;
}

// fd[0] = read;
// fd[1] = write;
// void	piping(t_lexer **lexer)
// {
// 	t_pipedata	data;
// 	int			pid;
// 	int			check;			// temporary test to fix output redirections -- seems to have broken input redirection as well tho

// 	check = -1;
// 	data.lexer = lexer;
// 	initialize_pipedata(&data);
// 	while (lexer[data.lex_count])
// 		data.lex_count++;
// 	if (strcmp(lexer[0]->tokenid, "<") == 0)
// 	{
// 		data.input_fd = redirection_handler(lexer[0]);
// 		data.j++;
// 	}
// 	if (data.lex_count > 1 && (strcmp(lexer[data.lex_count - 1]->tokenid, ">") == 0 || strcmp(lexer[data.lex_count - 1]->tokenid, ">>") == 0))
// 	{
// 		data.output_fd = redirection_handler(lexer[data.lex_count - 1]);
// 		data.lex_count -= 1;  // Adjust the lex_count to not include the NULL lexer
// 	}
// 	while (data.j < data.lex_count)		// && lexer[data.j]->cmd != NULL
// 	{
// 		printf("start of while loop where j = %d\n", data.j);
// 		// Handle any middle redirections
// 		// set_redirection(lexer, &data.input_fd, data.j);
// 		if (pipe(data.fd) == -1)
// 			return ;
// 		pid = fork();
// 		printf("pid generated\n");
// 		if (pid == -1)
// 			return ;
// 		if (pid == 0 && lexer[data.j]->already_execd == false)  // Child process ------- ! ! ! && check != data.j breaks the normal pipes, need to find a better difference between first and second iteration of same lexer
// 		{
// 			printf("executing child process\n");
// 			lexer[data.j]->already_execd = true;
// 			execute_child_process(&data);
// 		}
// 		else  // Parent process
// 		{
// 			printf("parent process after child termination\n");
// 			close(data.fd[1]);
// 			if (data.j != 0)
// 				close(data.prev_fd);
// 			data.prev_fd = data.fd[0];
// 		}
// 		if (check == data.j)
// 			data.j++;									// so here we increment to the next lexer; need conditions to not increase if cmd & file != NULL
// 		check = data.j;
// 		if (data.input_fd != -1)
// 		{
// 			close(data.input_fd);
// 			data.input_fd = -1;
// 			data.j++;
// 		}
// 	}
// 	close(data.prev_fd);
// 	data.j = 0;
// 	while (data.j < data.lex_count)
// 	{
// 		printf("waiting for child process termination");
// 		wait(NULL);  // Wait for all child processes to finish
// 		data.j++;
// 	}
// }


void	piping(t_lexer **lexer)
{
	t_pipedata	data;
	int			pid;
	int			status;

	initialize_pipedata(&data);
	data.lexer = lexer;
	while (lexer[data.lex_count] != NULL)
	{
		pipe(data.fd);
		pid = fork();
		if (pid == -1)
		{
			perror("Fork failed");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			if (data.input_fd != -1)
            {
                dup2(data.input_fd, STDIN_FILENO);
                close(data.input_fd);
            }
			if (lexer[data.lex_count]->tokenid[0] == '<')
			{
				data.fd[0] = redirection_handler(lexer[data.lex_count]);
				dup2(data.fd[0], STDIN_FILENO);
				dup2(data.fd[1], STDOUT_FILENO);
				data.lex_count++;
			}
			else if (lexer[data.lex_count]->tokenid[0] == '>')
			{
				data.fd[1] = redirection_handler(lexer[data.lex_count]);
				dup2(data.fd[1], STDOUT_FILENO);
			}
			else if (data.lex_count != 0 && lexer[data.lex_count + 1] != NULL)
			{
				dup2(data.input_fd, STDIN_FILENO);
				close(data.input_fd);
				dup2(data.fd[1], STDOUT_FILENO);
			}
			else if (data.lex_count != 0 )
			{
				dup2(data.input_fd, STDIN_FILENO);
				close(data.input_fd);
			}
			else
				dup2(data.fd[1], STDOUT_FILENO);
			close(data.fd[0]);
			close(data.fd[1]);
			if (lexer[data.lex_count]->cmd)
				execute_child_process(&data);
		}
		else  // Parent process
		{
			close(data.fd[1]); // Close writing end as we are reading from the pipe
			if ((lexer[data.lex_count + 1] == NULL || lexer[data.lex_count + 1]->cmd == NULL) && lexer[data.lex_count]->tokenid[0] == '<')
				close(data.fd[0]);
			else
				data.input_fd = data.fd[0];
			waitpid(pid, &status, 0);
		}
		data.lex_count++;
	}
}


			// else if (data.lex_count != 0)  // If not the first command and no input redirection
			// {
			// 	puts("should show up");
        	// 	dup2(data.input_fd, STDIN_FILENO);
			// 	close(data.input_fd);
			// 	if (lexer[data.lex_count] != NULL)
			// 		if (lexer[data.lex_count]->cmd != NULL)
			// 			dup2(data.fd[1], STDOUT_FILENO);
			// }


			// else if (data.input_fd == -1)
			// {
			// 	data.input_fd = data.fd[0];
			// 	dup2(data.input_fd, STDIN_FILENO);
			// 	close(data.input_fd);
			// }
			// else if (data.input_fd != -1)
			// {
			// 	puts("test");
			// 	dup2(data.input_fd, STDIN_FILENO);
			// }
			// if (data.lexer[data.lex_count + 1]->cmd != NULL)
        	// 	dup2(data.fd[1], STDOUT_FILENO);
