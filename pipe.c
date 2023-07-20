/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 15:16:04 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/07/20 15:31:28 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_lexer
{
	char tokenid[3];
	char *cmd;
	char *flags;
	char *args;
	// char *content;
	int	possition;
	
}	t_lexer;

/*
create our own getPath, systemcall env grep path output, split on ":", loop through all commands until 1 matches (strjoin or ls somehow...)
bool	is_cmd_valid(char *str); 
*/

// write(2...)2 is STDERR
void	exec (t_lexer *cmd, char **env)
{
	char	**args;
	char	*path;

	args = ft_split(cmd->content, ' ');
	if (ft_strchr(args[0], '/') > -1)
		path = args[0];
	else
		path = getPath(args[0], env);
	execve(path, args, env);
	write(2, "pipex: ", 7);
	write(2, cmd->content, ft_strlen(cmd->content));
	write(2, ": command not found\n", 20);
	exit(127);					// 127 is returned when exec command not found in PATH system variable (https://stackoverflow.com/questions/1763156/127-return-code-from)
}

void	redir(t_lexer *cmd, char **env, int fdin)
{
	pid_t	pid;
	int		pipefd[2];			//pipefd[0] = read;  pipefd[1] = write;

	pipe(pipefd);
	pid = fork();				// create a new process
	if (pid)
	{
		close(pipefd[1]);
		dup2(pipefd[0], 0);		// duplicate fd
		waitpid(pid, NULL, 0);	// wait for process termination
	}
	else
	{
		close(pipefd[0]);
		dup2(pipefd[1], 1);
		if (fdin == 0)
			exit(1);
		else
			exec(cmd->content, env);
	}
}

int	check_empty(char *str)
{
	int	i = 0;
	while (str[i])
		i++;
	if (i != 0)
		return (0);
	return (1);
}


// F_OK is to check that file exists
// write (2, ..., ...) 2 is error.
// open flags are for safety, https://man7.org/linux/man-pages/man2/open.2.html
int	open_file(char *filename, int mode)
{
	if (mode == 0)
	{
		if (access(filename, F_OK) < 0)
		{
			write(2, ": No such file or directory\n", 28);
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
 int	piping(t_lexer **lexer, char **env)
{
	int	fdin;
	int fdout;
	int	i;

	i = 4;
	if (check_empty(argv[1]) || check_empty(argv[2]))
		return (0);
	// !!not in an if statement, assuming if it enters piping it's good to be piped.
	fdin = open_file(argv[0], 0);
	fdout = open_file(argv[3], 1);
	dup2(fdin, 0);
	dup2(fdout, 1);
	redir(argv[3], env, fdin);
	while (i < argc - 2)
	{
		redir(argv[i], env, 1);
		i += 2;
	}
	exec(argv[i], env);
}

// int	main(int argc, char **argv)
// {

// }

// < file 1 cmd 1 | cmd 2 | cmd 3 | cmd 4 | cmd 5 > file 2