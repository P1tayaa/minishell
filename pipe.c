/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 15:16:04 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/08/16 19:59:49 by oscarmathot      ###   ########.fr       */
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

/*
1. The first time you call `strtok`, you pass the string you want to tokenize. `strtok` finds the first delimiter
in the string, replaces it with a null terminator (`\0`), and returns a pointer to the beginning of the string.
This effectively gives you the first token.
2. In subsequent calls, you pass `NULL` as the first argument. `strtok` remembers the string from the previous
call and continues tokenizing from where it left off. It finds the next delimiter, replaces it with a null terminator,
and returns a pointer to the next token.
3. When there are no more tokens, `strtok` returns `NULL`.
This behavior is standard for the `strtok` function in C. Your implementation, `ft_strtok`,
appears to mimic this behavior with the `last_token` static variable. So, passing `NULL` to `ft_strtok` in
subsequent calls is the correct approach to continue tokenizing the same string.
*/

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
		printf("\n%s\n", full_path);
		if (access(full_path, X_OK) == 0)
		{
			free(tmp_path);
			return (strdup(full_path));
		}
		token = ft_strtok(NULL, ":");						// segfault triggers here, but all standards say that this is fine? check strtok implementation
	}
	free(tmp_path);
	return (NULL);
}

void	exec(t_lexer *lexer)
{
	char	*args[4];									// Assuming a maximum of one flag and one argument for simplicity.
	char	*cmd_path;

	cmd_path = get_cmd_path(lexer->cmd);
	if (cmd_path == NULL)
	{
		perror("Command not found EXEC");
		return ;
	}
	args[0] = lexer->cmd;
	args[1] = lexer->flags;
	args[2] = lexer->args;
	args[3] = NULL;
	if (fork() == 0)									// Child process
	{
		execve (cmd_path, args, environ);
		perror ("Execution failed");
		exit (EXIT_FAILURE);
	}
	else
		wait(NULL);  									// Parent waits for child to finish
	free(cmd_path);
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
	char	*cmd_path; 
	int		fd[2];
	int		fd_in = 0;  // We start with stdin
	int		i = 0;
	int		wait_counter;

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
			cmd_path = get_cmd_path(lexer[i]->cmd);
			char *args[4] = {lexer[i]->cmd, lexer[i]->flags, lexer[i]->args, NULL};
			execve(cmd_path, args, environ);
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
