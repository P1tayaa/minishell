/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sboulain <sboulain@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 15:16:04 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/09/12 15:04:40 by sboulain         ###   ########.fr       */
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

// fd[0] = read;
// fd[1] = write;
void	piping(t_lexer **lexer)
{
	char	*cmd_path;
	char	*args[4];
	int		fd[2];
	int		pid;
	int		lex_count = 0;
	int		i;
	int		j;
	int		input_fd = -1;
	int		output_fd = -1;
	int		prev_fd;	// To store the read end of the previous pipe

	while (lexer[lex_count])
		lex_count++;
	prev_fd = -1;
	j = 0;
	if (strcmp(lexer[0]->tokenid, "<") == 0)
	{
		input_fd = redirection_handler(lexer[0]);
		j++;
	}
	// Handle output redirection at the end
	if (lex_count > 1 && strcmp(lexer[lex_count - 1]->tokenid, ">") == 0)
	{
		output_fd = redirection_handler(lexer[lex_count - 2]);
		lex_count -= 2;  // Adjust the lex_count to not include the output redirection
	}
	while (j < lex_count)
	{
		// Handle any middle redirections
		set_redirection(lexer, &input_fd, j);
		if (pipe(fd) == -1)
			return ;
		pid = fork();
		if (pid == -1)
			return ;
		if (pid == 0)  // Child process
		{
			// If there's an input redirection, use it
			if (input_fd != -1)
			{
				dup2(input_fd, STDIN_FILENO);
				close(input_fd);
			}
			else if (j != 0)  // If not the first command and no input redirection
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			// If this is the last command and there's an output redirection, use it
			if (j == lex_count - 1 && output_fd != -1)
			{
				dup2(output_fd, STDOUT_FILENO);
				close(output_fd);
			}
			else if (j != lex_count - 1)  // Not the last command, set up for next command
				dup2(fd[1], STDOUT_FILENO);
			close(fd[0]);
			close(fd[1]);
			cmd_path = get_cmd_path(lexer[j]->cmd);
			i = 0;
			args[i++] = lexer[j]->cmd;
			if (lexer[j]->flags != NULL)
				args[i++] = lexer[j]->flags;
			if (lexer[j]->args != NULL)
				args[i++] = lexer[j]->args;
			args[i++] = NULL;
			execve(cmd_path, args, environ);
			perror("Execution failed");
			exit(EXIT_FAILURE);
		}
		else  // Parent process
		{
			close(fd[1]);
			if (j != 0)
				close(prev_fd);
			prev_fd = fd[0];
		}
		j++;
		if (input_fd != -1)
		{
			close(input_fd);
			input_fd = -1;
			j++;
		}
	}
	close(prev_fd);
	j = 0;
	while (j < lex_count)
	{
		wait(NULL);  // Wait for all child processes to finish
		j++;
	}
}
