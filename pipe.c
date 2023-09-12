/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 15:16:04 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/08/29 19:26:29 by oscarmathot      ###   ########.fr       */
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
	printf("cmd: (%s)\n", args[i++]);
	if (lexer->flags != NULL)
		printf("flags: (%s)\n", args[i++]);
	if (lexer->args != NULL)
		printf("args: (%s)\n", args[i++]);
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

int	redirection_handler(t_lexer *lexer)
{
	if (strcmp(lexer->tokenid, "<") == 0)
		return (open(lexer->file, O_RDONLY));
	else if (strcmp(lexer->tokenid, ">") == 0)
		return (open(lexer->file, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	else if (strcmp(lexer->tokenid, ">>") == 0)
		return (open(lexer->file, O_WRONLY | O_CREAT | O_APPEND, 0644));
	// TODO: <<
	return (-1);
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

void execute_child_process(t_pipedata *data) 
{
    char *cmd_path;
    char *args[4];
    int i = 0;

    // If there's an input redirection, use it
    if (data->input_fd != -1)
    {
        dup2(data->input_fd, STDIN_FILENO);
        close(data->input_fd);
    }
    else if (data->j != 0)  // If not the first command and no input redirection
    {
        dup2(data->prev_fd, STDIN_FILENO);
        close(data->prev_fd);
    }
    // If this is the last command and there's an output redirection, use it
    if (data->j == data->lex_count - 1 && data->output_fd != -1)
    {
        dup2(data->output_fd, STDOUT_FILENO);
        close(data->output_fd);
    }
    else if (data->j != data->lex_count - 1)  // Not the last command, set up for next command
        dup2(data->fd[1], STDOUT_FILENO);

    close(data->fd[0]);
    close(data->fd[1]);
    cmd_path = get_cmd_path(data->lexer[data->j]->cmd);
    args[i++] = data->lexer[data->j]->cmd;
    if (data->lexer[data->j]->flags != NULL)
        args[i++] = data->lexer[data->j]->flags;
    if (data->lexer[data->j]->args != NULL)
        args[i++] = data->lexer[data->j]->args;
    args[i++] = NULL;
    execve(cmd_path, args, environ);
    perror("Execution failed");
    exit(EXIT_FAILURE);
}

// fd[0] = read;
// fd[1] = write;
void	piping(t_lexer **lexer)
{
	t_pipedata	data;
	int			pid;

	while (lexer[data.lex_count])
		data.lex_count++;
	data.prev_fd = -1;
	data.input_fd = -1;
	data.output_fd = -1;
	data.j = 0;
	if (strcmp(lexer[0]->tokenid, "<") == 0)
	{
		data.input_fd = redirection_handler(lexer[0]);
		// lex_count--;
		data.j++;
	}
	// Handle output redirection at the end
	if (data.lex_count > 1 && (strcmp(lexer[data.lex_count - 1]->tokenid, ">") == 0 || strcmp(lexer[data.lex_count - 1]->tokenid, ">>") == 0))
	{
		data.output_fd = redirection_handler(lexer[data.lex_count - 2]);
		data.lex_count -= 2;  // Adjust the lex_count to not include the output redirection
	}
	while (data.j < data.lex_count)
    {
        // Handle any middle redirections
        set_redirection(lexer, &data.input_fd, data.j);
        if (pipe(data.fd) == -1)
            return;
        pid = fork();
        if (pid == -1)
            return;
        if (pid == 0)  // Child process
		{
            data.lexer = lexer;
			execute_child_process(&data);
		}
		else  // Parent process
		{
            close(data.fd[1]);
            if (data.j != 0)
                close(data.prev_fd);
            data.prev_fd = data.fd[0];
        }
        data.j++;
        if (data.input_fd != -1)
        {
            close(data.input_fd);
            data.input_fd = -1;
            data.j++;
        }
    }
    close(data.prev_fd);
    data.j = 0;
    while (data.j < data.lex_count)
    {
        wait(NULL);  // Wait for all child processes to finish
        data.j++;
    }
}
