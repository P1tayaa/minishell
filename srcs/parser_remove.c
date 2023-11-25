/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_remove.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 22:05:07 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 22:05:12 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	move_file_name_to_file_and_comand_back_p2(t_lexer *lexer, int i)
{
	char	*temp;
	int		j;

	temp = malloc(ft_strlen(lexer->args) - i + 1);
	if (!temp)
		exit (1);
	j = 0;
	while (lexer->args[i + j] != '\0')
	{
		temp[j] = lexer->args[i + j];
		j++;
	}
	temp[j] = '\0';
	free(lexer->args);
	lexer->args = temp;
}

/*
	Put the cmd from current lexer into file of the previus lexer.
	Than moves the first word from agrs back to cmd,
	in case cmd was actually a file.
*/
void	move_file_name_to_file_and_comand_back(t_lexer *lexer_previous,
		t_lexer *lexer)
{
	int	i;

	lexer_previous->file = lexer->cmd;
	if (lexer->args == NULL)
	{
		lexer->cmd = NULL;
		lexer->args = NULL;
		return ;
	}
	lexer->cmd = ft_strtok(lexer->args, " ");
	i = 0;
	if (lexer->args == NULL)
		return ;
	while (lexer->args[i] != ' ' && lexer->args[i] != '\0')
		i++;
	if (lexer->args[i] == '\0')
	{
		free(lexer->args);
		lexer->args = NULL;
		return ;
	}
	move_file_name_to_file_and_comand_back_p2(lexer, i);
}

/*
	Return a string with all the characters at the fronts that is a space.
	It frees the origial STR variable
*/
char	*remove_front_spaces(char *str)
{
	int		i;
	char	*str_temp;

	if (str == NULL)
		return (NULL);
	i = 0;
	while (ft_isspace(str[i]) && str[i] != '\0')
	{
		i++;
	}
	if (str[i] == '\0')
	{
		free(str);
		return (NULL);
	}
	str_temp = ft_strdup(&str[i]);
	if (!str_temp)
		exit(1);
	free(str);
	return (str_temp);
}

char	*remove_back_spaces_p2(char *str, int i, int how_many_char)
{
	char	*str_dup_without_spaces;

	str_dup_without_spaces = malloc(ft_strlen(str) - i + 1);
	if (!str_dup_without_spaces)
		exit (1);
	i = 0;
	while (i < how_many_char)
	{
		str_dup_without_spaces[i] = str[i];
		i++;
	}
	str_dup_without_spaces[i] = '\0';
	free(str);
	return (str_dup_without_spaces);
}

/*
	Return a string with all the characters at the back that is a space.
	It frees the origial STR variable
*/
char	*remove_back_spaces(char *str)
{
	char	*str_dup_without_spaces;
	int		i;
	int		how_many_char;

	if (str == NULL)
		return (NULL);
	i = 0;
	while (ft_isspace(str[ft_strlen(str) - i - 1]) && ft_strlen(str) - i > 0)
		i++;
	how_many_char = ft_strlen(str) - i;
	if (i == 0)
	{
		str_dup_without_spaces = ft_strdup(str);
		if (!str_dup_without_spaces)
			exit(1);
		free(str);
		return (str_dup_without_spaces);
	}
	if (ft_strlen(str) == (size_t)i)
	{
		free(str);
		return (NULL);
	}
	str_dup_without_spaces = remove_back_spaces_p2(str, i, how_many_char);
	return (str_dup_without_spaces);
}
