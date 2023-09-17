/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sboulain <sboulain@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 13:30:47 by sboulain          #+#    #+#             */
/*   Updated: 2023/09/12 14:57:29 by sboulain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		count_pipes(char *str);
char	**get_list_of_tokenid(void);
void	free_list_of_tokenid(char **list_of_tokenid);
int		count_token(char *input);
// int	copy_input_to_parcer_and_keep_track_of_curser(char *input, t_lexer *current_lex, int curser);
void	free_parser(t_lexer **lexer);
void	free_lexer(t_lexer **lexer);
int 	parse_until_token_id(char *input, int current_lex, t_lexer **lexer, int curser);


// figure out where ls -la | grep "." | cat > out -e > out2
// the -e should be
/*
	so it take the user input (string) and parse it into out lexer struct​
*/

int ft_char_find(char *str, const char *list_of_char);
bool	ft_isspace(unsigned char c);

int count_number_flags_char(int *args_char_len, t_lexer *lexer)
{
	int flags_char_len;
	int i;

	flags_char_len = 0;
	*args_char_len = 0;
	i = 0;
	while (lexer->args[i] != '\0')
	{
		if (i != 0)
			if (ft_isspace(lexer->args[i - 1]) && lexer->args[i] == '-')
			{
				while (!ft_isspace(lexer->args[i]) && lexer->args[i] != '\0')
				{
					flags_char_len++;
					i++;
				}
				flags_char_len++;
				continue ;
			}
		if (i == 0 && lexer->args[i] == '-')
		{
			while (!ft_isspace(lexer->args[i]) && lexer->args[i] != '\0')
				{
					// printf("%c", lexer->args[i]);
					flags_char_len++;
					i++;
				}
				flags_char_len++;
				continue ;
		}
		(*args_char_len)++;
		i++;
	}
	return (flags_char_len);
}

// void	copy_flags_from_args()
// {
	
// }

void	move_flags_from_args_to_flags(t_lexer *lexer)
{
	int	i;
	char *args;
	char *flags;
	int flags_char_len;
	int args_char_len;

	// count how many char are to be in flags and args
	flags_char_len = count_number_flags_char(&args_char_len, lexer);


	if (args_char_len != 0)
	{
		args = malloc(sizeof(char) * (args_char_len + 1));
		if (!args)
			exit (-1);
	}
	else
		args = NULL;
	flags = malloc(sizeof(char) * (flags_char_len + 1));
	if (!flags)
		exit (-1);

	// copy in them
	
	flags_char_len = 0;
	args_char_len = 0;
	i = 0;
	while (lexer->args[i] != '\0')
	{
		if (i != 0)
		{
			// printf("bool status %d", ft_isspace(lexer->args[i - 1]) && lexer->args[i] == '-');
			if (ft_isspace(lexer->args[i - 1]) && lexer->args[i] == '-')
			{
				while (!ft_isspace(lexer->args[i]) && lexer->args[i] != '\0')
				{
					flags[flags_char_len] = lexer->args[i];
					flags_char_len++;
					i++;
				}
				flags[flags_char_len] = ' ';
				flags_char_len++;
				continue ;
			}
		}
		if (i == 0 && lexer->args[i] == '-')
		{
			while (!ft_isspace(lexer->args[i]) && lexer->args[i] != '\0')
			{
				printf("added char to flags: %c\n", lexer->args[i]);
				flags[flags_char_len] = lexer->args[i];
				flags_char_len++;
				i++;
			}
			flags[flags_char_len] = ' ';
			flags_char_len++;
			continue ;
		}
		args[args_char_len] = lexer->args[i];
		args_char_len++;
		i++;
	}
	// puts("test4");
	flags[flags_char_len] = '\0';
	if (args != NULL)
		args[args_char_len] = '\0';

	// replace flags and args, but join previous flags
	// puts(flags);
	char *temp;
	free(lexer->args);
	lexer->args = args;
	if (lexer->flags != NULL)
	{
		temp = ft_strjoin(lexer->flags, flags);
		free(lexer->flags);
		free(flags);
		lexer->flags = temp;
	}
	else
		lexer->flags = flags;
}

void	move_file_name_to_file_and_comand_back(t_lexer *lexer_previous, t_lexer *lexer)
{
	int i;
	int	j;
	char *temp;

	lexer_previous->file = lexer->cmd;
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
	temp = malloc(ft_strlen(lexer->args) - i + 1);
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

char *remove_front_spaces(char *str)
{
	int i;
	char *str_temp;

	if (str == NULL)
		return NULL;
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
	free(str);
	return (str_temp);
}

char *remove_back_spaces(char *str)
{
	char *str_dup_without_spaces;
	int	i;
	int how_many_char;

	if (str == NULL)
		return NULL;
	i = 0;
	while (ft_isspace(str[ft_strlen(str) - i - 1]) && ft_strlen(str) - i > 0)
		i++;
	// if (str[0] == '-')
	// 	printf("%d spaces, %zu chars", i, ft_strlen(str));
	how_many_char = ft_strlen(str) - i;
	if (i == 0)
	{
		str_dup_without_spaces = ft_strdup(str);
		free(str);
		return (str_dup_without_spaces);
	}
	if (ft_strlen(str) == (size_t)i)
	{
		free(str);
		return (NULL);
	}
	str_dup_without_spaces = malloc(ft_strlen(str) - i + 1);
	if (!str_dup_without_spaces)
		exit (-1);
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

t_lexer	**parsse_things(char *str)
{
	int	token_num;
	int curser;
	int	i;
	t_lexer **lexer;

	token_num = count_token(str);
	lexer = (t_lexer **)malloc(sizeof(t_lexer *) * (token_num + 2));
	if (!lexer)
		exit(1);
	i = 0;
	curser = 0;
	while (i <= token_num)
	{
		lexer[i] = (t_lexer *)malloc(sizeof(t_lexer));
		if (!lexer[i])
			exit(1);
		lexer[i]->possition = i;
		lexer[i]->args = NULL;
		lexer[i]->cmd = NULL;
		lexer[i]->file = NULL;
		lexer[i]->flags = NULL;
		lexer[i]->tokenid[0] = '\0';
		curser = parse_until_token_id(str, i, lexer, curser);
		// * need to check if flags are writen before the function. 
		// * If so put it to the previous function
		puts("Test");
		if (ft_char_find(lexer[i]->args, "-") != -1)
		{
			move_flags_from_args_to_flags(lexer[i]);
		}
		puts("test1");
		if (i > 0)
		{
			if (lexer[i - 1]->tokenid[0] == '<' && lexer[i - 1]->tokenid[1] == '\0')
				move_file_name_to_file_and_comand_back(lexer[i - 1], lexer[i]);
			if (lexer[i - 1]->tokenid[0] == '<' && lexer[i - 1]->tokenid[1] == '<')
			{
				lexer[i - 1]->args = lexer[i]->cmd;
				lexer[i]->cmd = NULL;
			}
			if (lexer[i - 1]->tokenid[0] == '>')
			{
				lexer[i - 1]->file = lexer[i]->cmd;
				lexer[i]->cmd = NULL;
			}
		}
		// puts(lexer[i]->args);
		lexer[i]->args = remove_front_spaces(lexer[i]->args);
		lexer[i]->cmd = remove_front_spaces(lexer[i]->cmd);
		lexer[i]->file = remove_front_spaces(lexer[i]->file);
		lexer[i]->flags = remove_front_spaces(lexer[i]->flags);
		lexer[i]->args = remove_back_spaces(lexer[i]->args);
		lexer[i]->cmd = remove_back_spaces(lexer[i]->cmd);
		lexer[i]->file = remove_back_spaces(lexer[i]->file);
		// puts(lexer[i]->flags);
		lexer[i]->flags = remove_back_spaces(lexer[i]->flags);
		// puts(lexer[i]->flags);
		i++;
	}
	lexer[i] = NULL;
	i = 0;
	return (lexer);
}

int	count_char_until_next_token(char *input);

bool	is_a_token_id(char *input)
{
	char	**list_of_tokenid;
	int		i;
	int		j;

	// puts("test");
	list_of_tokenid = get_list_of_tokenid();
	i = 0;
	// puts(list_of_tokenid[1]);
	while (list_of_tokenid[i] != NULL)
	{
		j = 0;
		while (input[j] != '\0' && list_of_tokenid[i][j] == input[j])
		{
			j++;
		}
		if (list_of_tokenid[i][j] == '\0')
		{
			free_list_of_tokenid(list_of_tokenid);
			// puts("return true");
			return (true);
		}
		i++;
	}
	free_list_of_tokenid(list_of_tokenid);
	return (false);
}

int	copy_until_space(int i, char *input_after_curser, char **destination)
{
	int	j;

	j = 0;
	while (input_after_curser[i + j] != '\0')
	{
		if ((ft_isspace(input_after_curser[i + j])
				|| is_a_token_id(&input_after_curser[i + j])))
			break ;
		j++;
	}
	if (j == 0)
		return (i + j);
	(*destination) = (char *)malloc(sizeof(char) * (j + 2));
	if (!(*destination))
		exit(1);
	j = 0;
	while (input_after_curser[i + j] != '\0')
	{
		if ((ft_isspace(input_after_curser[i + j])
				|| is_a_token_id(&input_after_curser[i + j])))
			break ;
		(*destination)[j] = input_after_curser[i + j];
		j++;
	}
	(*destination)[j] = '\0';
	return (i + j);
}

int	copy_until_tokenid(int i, char *input_after_curser, char **destination)
{
	int	j;

	j = 0;
	while (input_after_curser[i + j] != '\0')
	{
		if (is_a_token_id(&input_after_curser[i + j]))
			break ;
		j++;
	}
	if (j == 0)
		return (i + j);
	(*destination) = (char *)malloc(sizeof(char) * (j + 2));
	if (!(*destination))
		exit(1);
	j = 0;
	while (input_after_curser[i + j] != '\0')
	{
		if (is_a_token_id(&input_after_curser[i + j]))
			break ;
		(*destination)[j] = input_after_curser[i + j];
		j++;
	}
	(*destination)[j] = '\0';
	return (i + j);
}

int	write_the_right_token(int i, char *input_after_curser,char tokenid[3])
{
	int	j;

	j = 0;
	while (input_after_curser[i + j] != '\0'
		&& !ft_isspace(input_after_curser[i + j])
		&& is_a_token_id(&input_after_curser[i + j]) && j < 2)
	{
		(tokenid)[j] = input_after_curser[i + j];
		j++;
	}
	(tokenid)[j] = '\0';
	return (i + j);
}

int parse_until_token_id(
		char *input, int current_lex, t_lexer **lexer, int curser)
{
	int		i;
	bool	function_done;

	i = 0;
	function_done = false;
	while (input[i + curser] != '\0')
	{
		// printf("%d = i\n", curser + i);
		// if (function_done && input[i + curser] == '-')
		// {
		// 	i = copy_until_space(i, &input[curser], &lexer[current_lex]->flags);
		// 	// continue ;
		// }
		if (function_done == false && ft_isspace(input[i + curser]) == false)
		{
			i = copy_until_space(i, &input[curser], &lexer[current_lex]->cmd);
			function_done = true;
			// printf("curser + i = %d", i + curser);
			// continue ;
		}
		if (function_done && ft_isspace(input[i + curser]) == false)
		{
			i = copy_until_tokenid(i, &input[curser], &(lexer[current_lex]->args));
			// continue ;
		}
		if (is_a_token_id(&input[i + curser]) == true)
		{
			// puts("arrive!");
			i = write_the_right_token(i, &input[curser], (lexer[current_lex]->tokenid));
			break ;
		}
		if (ft_isspace(input[i + curser]))
			i++;
	}
	// puts(lexer[current_lex]->args);
	return (curser + i);
}

int	count_char_until_next_token(char *input)
{
	char	**list_of_tokenid;
	int		i;
	int		j;
	int		k;

	list_of_tokenid = get_list_of_tokenid();
	i = 0;
	while (input[i] != '\0')
	{
		j = 0;
		while (list_of_tokenid[j] != NULL)
		{
			k = 0;
			while (list_of_tokenid[j][k] == input[i + k])
			{
				k++;
			}
			if (list_of_tokenid[j][k] == '\0')
			{
				free_list_of_tokenid(list_of_tokenid);
				return (i);
			}
			j++;
		}
		i++;
	}
	free_list_of_tokenid(list_of_tokenid);
	return (i);
}

// I think this is lower then n^2 complexity, log(n) I think
int	count_token(char *input)
{
	char	**list_of_tokenid;
	int		num_of_token;
	int		i;
	int		j;
	int		k;

	list_of_tokenid = get_list_of_tokenid();
	num_of_token = 0;
	i = 0;
	while (input[i] != '\0')
	{
		j = 0;
		while (list_of_tokenid[j] != NULL)
		{
			k = 0;
			while (input[i + k] != '\0' &&list_of_tokenid[j][k] == input[i + k])
			{
				k++;
			}
			if (list_of_tokenid[j][k] == '\0')
			{
				i = i + k;
				// printf(" i = %d, list_of_tokenid %s\n",i , list_of_tokenid[j]);
				num_of_token++;
				break ;
			}
			j++;
		}
		i++;
	}
	free_list_of_tokenid(list_of_tokenid);
	return (num_of_token);
}
