/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 15:16:04 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/08/25 13:58:37 by oscarmathot      ###   ########.fr       */
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
	token = strtok(tmp_path, ":");
	while (token)
	{
		concat_path(full_path, token, cmd);
		// printf("%s\n", full_path);
		if (access(full_path, X_OK) == 0)
		{
			free(tmp_path);
			return (strdup(full_path));
		}
		token = strtok(NULL, ":");						// segfault triggers here, ft_strtok's got something wrong so using official library AT THE MOMENT ONLY
	}
	free(tmp_path);
	return (NULL);
}


// test for command and garbage argument.
void	exec(t_lexer *lexer)
{
	char	*args[4];  // Assuming a maximum of one flag and one argument for simplicity.
	char	*cmd_path;
	int		i;
	// char	*cwd[1024];
	pid_t	pid;

	cmd_path = get_cmd_path(lexer->cmd);
	if (cmd_path == NULL)
	{
		perror("Command not found EXEC");
		return ;
	}
	// cwd = getcwd
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

// fd[0] = read;
// fd[1] = write;
void piping(t_lexer **lexer)
{
    char	*cmd_path;
    char	*args[4];
    int		fd[2];
    int		previous_fd[2] = {-1, -1};
    int		pid;
    int		lex_count = 0;
    int		i;
	int		j;

    // Determine the number of commands in the pipeline
	while (lexer[lex_count])
		lex_count++;
	j = 0;
	while (j < lex_count)
	{
		if (pipe(fd) == -1)
			return;
		pid = fork();
		if (pid == -1)
			return;
		if (pid == 0)  // Child process
		{
			cmd_path = get_cmd_path(lexer[j]->cmd);
			i = 0;
			args[i++] = lexer[j]->cmd;
			if (lexer[j]->flags != NULL)
				args[i++] = lexer[j]->flags;
			if (lexer[j]->args != NULL)
				args[i++] = lexer[j]->args;
			args[i++] = NULL;
			if (j != 0)  // Not the first command
				dup2(previous_fd[0], STDIN_FILENO);
			if (j != lex_count - 1)  // Not the last command
				dup2(fd[1], STDOUT_FILENO);
			close(fd[0]);
			close(fd[1]);
			if (previous_fd[0] != -1)
				close(previous_fd[0]);
			if (previous_fd[1] != -1)
				close(previous_fd[1]);
			execve(cmd_path, args, environ);
			perror("Execution failed");
			exit(EXIT_FAILURE);
		}
        // Parent: Close unused ends of the pipes and store current pipe for the next iteration
		if (previous_fd[0] != -1)
			close(previous_fd[0]);
		if (previous_fd[1] != -1)
			close(previous_fd[1]);
		previous_fd[0] = fd[0];
		previous_fd[1] = fd[1];
		j++;
	}
	close(fd[0]);
	close(fd[1]);
	j = 0;
	while (j < lex_count)
	{
		wait(NULL);  // Wait for all child processes to finish
		j++;
	}
}


// void	piping(t_lexer **lexer)
// {
// 	char	*cmd_path;
// 	char	*args[4];
// 	int		fd[2];
// 	int		pid1;
// 	int		pid2;
// 	int		lex_count;
// 	int		i;

// 	lex_count = 0;
// 	i = 0;
// 	if (pipe(fd) == -1)
// 		return ;
// 	pid1 = fork();
// 	if (pid1 == -1)
// 		return ;
// 	if (pid1 == 0)
// 	{
// 		cmd_path = get_cmd_path(lexer[lex_count]->cmd);
// 		args[i++] = lexer[lex_count]->cmd;
// 		if (lexer[lex_count]->flags != NULL)
// 			args[i++] = lexer[lex_count]->flags;
// 		if (lexer[lex_count]->args != NULL)
// 			args[i++] = lexer[lex_count]->args;
// 		i = 0;
// 		printf("cmd: (%s)\n", args[i++]);
// 		if (lexer[lex_count]->flags != NULL)
// 			printf("flags: (%s)\n", args[i++]);
// 		if (lexer[lex_count]->args != NULL)
// 			printf("args: (%s)\n", args[i++]);
// 		printf("cmd1 path: %s\n", cmd_path);
// 		args[i++] = NULL;
// 		dup2(fd[1], STDOUT_FILENO);
// 		close(fd[0]);
// 		close(fd[1]);
// 		execve(cmd_path, args, environ);
// 	}
// 	// printf("\n");
// 	lex_count++;
// 	i = 0;
// 	pid2 = fork();
// 	if (pid2 == -1)
// 		return ;
// 	if (pid2 == 0)
// 	{
// 		cmd_path = get_cmd_path(lexer[lex_count]->cmd);
// 		args[i++] = lexer[lex_count]->cmd;
// 		if (lexer[lex_count]->flags != NULL)
// 			args[i++] = lexer[lex_count]->flags;
// 		if (lexer[lex_count]->args != NULL)
// 			args[i++] = lexer[lex_count]->args;
// 		args[i++] = NULL;
// 		i = 0;
// 		printf("cmd: (%s)\n", args[i++]);
// 		if (lexer[lex_count]->flags != NULL)
// 			printf("flags: (%s)\n", args[i++]);
// 		if (lexer[lex_count]->args != NULL)
// 			printf("args: (%s)\n", args[i++]);
// 		printf("cmd2 path: %s\n", cmd_path);
// 		dup2(fd[0], STDIN_FILENO);
// 		close(fd[0]);
// 		close(fd[1]);
// 		execve(cmd_path, args, environ);
// 	}
// 	close(fd[0]);
// 	close(fd[1]);
// 	waitpid(pid1, NULL, 0);
// 	waitpid(pid2, NULL, 0);
// }