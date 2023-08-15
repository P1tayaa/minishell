/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 15:16:04 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/08/15 13:01:00 by oscarmathot      ###   ########.fr       */
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
			return strdup(full_path);
		}
		token = ft_strtok(NULL, ":");
	}
	free(tmp_path);
	return (NULL);
}

void	exec(t_lexer *lexer)
{
	char	*args[4];  // Assuming a maximum of one flag and one argument for simplicity.

	args[0] = lexer->cmd;
	args[1] = lexer->flags;
	args[2] = lexer->args;
	args[3] = NULL;
	if (fork() == 0)
	{  // Child process
		execve (args[0], args, NULL);
		perror ("Execution failed");
		exit (EXIT_FAILURE);
	}
	else
		wait(NULL);  // Parent waits for child to finish
}

int	redir(t_lexer **lexer, int i)
{
	int fd;

	if (strcmp(lexer[i]->tokenid, "<") == 0)
	{
		fd = open_file(lexer[i]->cmd, 0);  // Open for reading
		if (fd == -1)
		{
			perror("Error opening file for reading");
			return (-1);
		}
		dup2(fd, 0);  // Redirect standard input to the file
		close(fd);
	}
	else if (strcmp(lexer[i]->tokenid, ">") == 0)
	{
		fd = open_file(lexer[i]->cmd, 1);
		if (fd == -1)
		{
			perror("Error opening file for writing");
			return (-1);
		}
		dup2(fd, 1);  // Redirect standard output to the file
		close(fd);
	}
	else if (strcmp(lexer[i]->tokenid, ">>") == 0)
	{
		fd = open(lexer[i]->cmd, O_WRONLY | O_CREAT | O_APPEND, 
					S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);  // Open for writing, create if doesn't exist, append if exists
		if (fd == -1)
		{
			perror("Error opening file for appending");
			return (-1);
		}
		dup2(fd, 1);  // Redirect standard output to the file
		close(fd);
	}
	// TODO : <<
    return (0);
}

void	piping(t_lexer **lexer)
{
	int fd[2];
	int fd_in = 0;  // We start with stdin
	int i = 0;
	int	wait_counter;

	while (lexer[i])
	{
		if (lexer[i+1] && strcmp(lexer[i+1]->tokenid, "|") == 0)	        	// if "|" pipe
			pipe(fd);
		if (fork() == 0)														// child process
		{
			if (i != 0)									        				// Not the first command, set up the input from the previous command
			{
				dup2(fd_in, 0);
				close(fd_in);
			}
			if (lexer[i+1] && strcmp(lexer[i+1]->tokenid, "|") == 0) 			// If pipe, set up the output to the next command
			{
				dup2(fd[1], 1);
				close(fd[1]);
			}
			char *args[4] = {lexer[i]->cmd, lexer[i]->flags, lexer[i]->args, NULL};
			execve(args[0], args, NULL);
			perror("Execution failed");
			exit (EXIT_FAILURE);
		}
		else															  		// Parent process
		{
			if (lexer[i+1] && strcmp(lexer[i+1]->tokenid, "|") == 0)
			{
				close(fd[1]);
				fd_in = fd[0];
			}
		}
		i += 2;  // Skip the next token since it's a pipe operator
	}
	wait_counter = i / 2;		
	while (wait_counter--)														// Parent waits for all child processes to finish
		wait(NULL);
}
