/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 15:16:04 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/08/14 20:21:22 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
extern char	**environ;

char	*get_cmd_path(const char *cmd)
{
	char	*path;
	char	*tmp_path;
	char	*token;
	char	full_path[1024];
	int		i;

	path = NULL;
	i = 0;
    // Search for the PATH variable in the environment
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
    args[3] = NULL;  // Null-terminate the args array
    if (fork() == 0)
	{  // Child process
		execve (args[0], args, NULL);  // Using execvp so we don't need to provide the full path
		perror ("Execution failed");  // perror will print the specific error
		exit (EXIT_FAILURE);
    }
	else
        wait(NULL);  // Parent waits for child to finish
}

// void	redir(t_lexer **cmd, char **env, int fdin)
// {
// 	pid_t	pid;
// 	int		pipefd[2];			//pipefd[0] = read;  pipefd[1] = write;

// 	pipe(pipefd);
// 	pid = fork();				// create a new process
// 	if (pid)
// 	{
// 		close(pipefd[1]);
// 		dup2(pipefd[0], 0);		// duplicate fd
// 		waitpid(pid, NULL, 0);	// wait for process termination
// 	}
// 	else
// 	{
// 		close(pipefd[0]);
// 		dup2(pipefd[1], 1);
// 		if (fdin == 0)
// 			exit(1);
// 		else
// 			exec(lexer[0]->cmd);
// 	}
// }

// F_OK is to check that file exists
// write (2, ..., ...) 2 is error.
// open flags are for safety, https://man7.org/linux/man-pages/man2/open.2.html
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

// < file 1 cmd 1 | cmd 2 > file 2
// should I use fork / are we allowe to fork?
// mode : 0 = INFILE, 1 = OUTFILE
void	piping(t_lexer **lexer)
{
    int fd[2];
    int fd_in = 0;  // We start with stdin
    int i = 0;
	int	wait_counter;

    while (lexer[i] && strcmp(lexer[i]->tokenid, "C") == 0)
	{
        // Check if the next command exists to decide on setting up a pipe
        if (lexer[i+1] && strcmp(lexer[i+1]->tokenid, "C") == 0)
            pipe(fd);
        if (fork() == 0)		// Child process
		{
            // Not the first command, set up the input from the previous command
            if (i != 0)
			{
                dup2(fd_in, 0);
                close(fd_in);
            }
            // Not the last command, set up the output to the next command
            if (lexer[i+1] && strcmp(lexer[i+1]->tokenid, "C") == 0)
			{
                dup2(fd[1], 1);
                close(fd[1]);
            }
            char *args[4] = {lexer[i]->cmd, lexer[i]->flags, lexer[i]->args, NULL};		// dunno how else to get this done
            execvp(args[0], args);
            perror("Execution failed");
            exit (EXIT_FAILURE);
        }
		else
		{  // Parent process
            if (lexer[i+1] && strcmp(lexer[i+1]->tokenid, "C") == 0)
			{
                close(fd[1]);
                fd_in = fd[0];
            }
        }
        i++;
    }
    // Parent waits for all child processes to finish
    wait_counter = i;
    while (wait_counter--)
        wait(NULL);
}

// < file 1 cmd 1 | cmd 2 | cmd 3 | cmd 4 | cmd 5 > file 2

/*
----------------------------------------------------------------------------------------------------
lexer[0]

ls -l | grep poop

1. token ID - the type of prompts
2. 

< file 1 cmd 1 -flags | cmd 2 | cmd 3 -flags | cmd 4 | cmd 5 > file 2

function (lexer, curese)
{
	des truc lexer[curse];
	function(lexer, cureser + 1)
}

sysmtem("env")
malloc (sizeof(char) * env);

execve(ls) -l 

*/ 