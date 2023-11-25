/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 15:16:04 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 23:26:50 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		are_there_more_cmds(t_lexer **lexer, int current);
char	*sjoin_fr(char *s1, char *s2);
char	***get_env(void);
void	ascii_sort(char **environment);
bool	is_str_export(char *str);
void	lexer_free(t_lexer **lexer);
void	restore_default_sigint_handling(void);
int		manage_reads_writes(t_pipedata *data, t_lexer **lexer);
char	*doll_to_num(char *str_og, int number_replace);

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

char	*check_path(t_pipedata *data)
{
	char	*path;
	int		i;

	path = NULL;
	i = 0;
	while ((*data->environ)[i] != NULL)
	{
		if (ft_memcmp((*data->environ)[i], "PATH=", 5) == 0)
		{
			path = (*data->environ)[i] + 5;
			break ;
		}
		i++;
	}
	if (!path)
	{
		write(2, "PATH not found in environment\n", 30);
		return (NULL);
	}
	return (ft_strdup(path));
}

char	*get_cmd_path(const char *cmd, t_pipedata *data)
{
	char	*path;
	char	*tmp_path;
	char	*token;
	char	full_path[1024];

	path = check_path(data);
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
	free(path);
	return (NULL);
}

int	redirection_handler(t_lexer *lexer)
{
	int	fd;

	fd = -1;
	if (ft_memcmp(lexer->tokenid, "<", 1) == 0
		&& ft_memcmp(lexer->tokenid, "<<", 2) != 0)
		fd = open(lexer->file, O_RDONLY);
	else if (ft_memcmp(lexer->tokenid, ">", 1) == 0
		&& ft_memcmp(lexer->tokenid, ">>", 2) != 0)
		fd = open(lexer->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (ft_memcmp(lexer->tokenid, ">>", 2) == 0)
	{
		if (access(lexer->file, F_OK) != -1)
			fd = open(lexer->file, O_WRONLY | O_APPEND, 0644);
		else
			fd = open(lexer->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	else if (ft_memcmp(lexer->tokenid, "<<", 2) == 0)
		fd = open(lexer->args, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell: ");
		exit(EXIT_FAILURE);
	}
	return (fd);
}

char	**split_spaces(char *str);
void	export_content_freeur(char ***export_content);

void	free_mk_args(t_pipedata *data, char ***args_temp)
{
	if (data->lexer[data->lex_count]->args != NULL)
		free((*args_temp));
}
