/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 13:30:47 by sboulain          #+#    #+#             */
/*   Updated: 2023/11/20 16:29:16 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		count_pipes(char *str);
char	**get_list_of_tokenid(void);
void	free_double_array(char **list_of_tokenid);
int		count_token(char *input);
void	free_parser(t_lexer **lexer);
void	free_lexer(t_lexer **lexer);
int		parse_until_token_id(char *input, int current_lex,
			t_lexer **lexer, int curser);
int		ft_char_find(char *str, const char *list_of_char);
bool	ft_isspace(unsigned char c);

void	count_flags_char_len(int *i, int *flags_char_len, t_lexer *lexer)
{
	while (!ft_isspace(lexer->args[(*i)]) && lexer->args[(*i)] != '\0')
	{
		(*flags_char_len)++;
		(*i)++;
	}
	(*flags_char_len)++;
}

/*
	Return the number of charaters that are going to be moved from arg to flags,
	And keeps track of how many are going to be left in arg with ARGS_CHAR_LEN
*/
int	count_number_flags_char(int *args_char_len, t_lexer *lexer)
{
	int	flags_char_len;
	int	i;

	flags_char_len = 0;
	*args_char_len = 0;
	i = 0;
	while (lexer->args[i] != '\0')
	{
		if (i != 0)
		{
			if (ft_isspace(lexer->args[i - 1]) && lexer->args[i] == '-')
			{
				count_flags_char_len(&i, &flags_char_len, lexer);
				continue ;
			}
		}
		if (i == 0 && lexer->args[i] == '-')
			count_flags_char_len(&i, &flags_char_len, lexer);
		else
		{
			(*args_char_len)++;
			i++;
		}
	}
	return (flags_char_len);
}

/*
	Malloc in new args and flags with the pointer
	to the address given in ARGS and FLAGS
*/
void	malloc_new_args_and_new_flags(int args_char_len, char **args,
		char **flags, int flags_char_len)
{
	if (args_char_len != 0)
	{
		(*args) = malloc(sizeof(char) * (args_char_len + 1));
		if (!(*args))
			exit (-1);
	}
	else
		(*args) = NULL;
	(*flags) = malloc(sizeof(char) * (flags_char_len + 2));
	if (!(*flags))
		exit (-1);
}

void	move_flag_from_arg_to_flags(int *i, int *flags_char_len,
		t_lexer *lexer, char **flags)
{
	(*i)++;
	while (!ft_isspace(lexer->args[(*i)]) && lexer->args[(*i)] != '\0')
	{
		(*flags)[(*flags_char_len)] = lexer->args[(*i)];
		(*flags_char_len)++;
		(*i)++;
	}
}

void	move_all_flag_from_arg_to_flags(char **flags,
		int args_char_len, char **args, t_lexer *lexer)
{
	int	i;
	int	flags_char_len;

	i = 0;
	flags_char_len = 1;
	while (lexer->args[i] != '\0')
	{
		if (i != 0)
		{
			if (ft_isspace(lexer->args[i - 1]) && lexer->args[i] == '-')
			{
				move_flag_from_arg_to_flags(&i, &flags_char_len, lexer, flags);
				continue ;
			}
		}
		if (i == 0 && lexer->args[i] == '-')
		{
			move_flag_from_arg_to_flags(&i, &flags_char_len, lexer, flags);
			continue ;
		}
		(*args)[args_char_len++] = lexer->args[i++];
	}
	(*flags)[flags_char_len] = '\0';
	if ((*args) != NULL)
		(*args)[args_char_len] = '\0';
}

/*
	Move the flags from args to there respective
	par of the lexer by reconising where the '-'
	
*/
void	move_flags_from_args_to_flags(t_lexer *lexer)
{
	char	*args;
	char	*flags;
	int		flags_char_len;
	int		args_char_len;
	char	*temp;

	flags_char_len = count_number_flags_char(&args_char_len, lexer);
	malloc_new_args_and_new_flags(args_char_len, &args, &flags, flags_char_len);
	flags[0] = '-';
	move_all_flag_from_arg_to_flags(&flags, 0, &args, lexer);
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

/*
	Initiates values to zero or null and malloc the new lexer i.
	
	LEXER is a pointer to the address of current one
	of double array of t_lexer (&lexer[i])
*/
void	initiate_values_to_zero_null(t_lexer **lexer, int i)
{
	(*lexer) = (t_lexer *)malloc(sizeof(t_lexer));
	if (!(*lexer))
		exit(1);
	(*lexer)->possition = i;
	(*lexer)->args = NULL;
	(*lexer)->cmd = NULL;
	(*lexer)->file = NULL;
	(*lexer)->flags = NULL;
	(*lexer)->tokenid[0] = '\0';
	(*lexer)->execd = false;
}

/*
	Removes the spaces that are at the front and back of
	all parts of LEXER
	by using remove_front_spaces() and remove_back_spaces()
	
	LEXER is a pointer to the address of current one
	of double array of t_lexer (&lexer[i])
*/
void	removes_front_and_back_redondant_spaces(t_lexer **lexer)
{
	(*lexer)->args = remove_front_spaces((*lexer)->args);
	(*lexer)->cmd = remove_front_spaces((*lexer)->cmd);
	(*lexer)->file = remove_front_spaces((*lexer)->file);
	(*lexer)->flags = remove_front_spaces((*lexer)->flags);
	(*lexer)->args = remove_back_spaces((*lexer)->args);
	(*lexer)->cmd = remove_back_spaces((*lexer)->cmd);
	(*lexer)->file = remove_back_spaces((*lexer)->file);
	(*lexer)->flags = remove_back_spaces((*lexer)->flags);
}

/*
	Depending of which one of tokenid (<,>,<<,>>).
	Place the first word of the user input (cmd) to the right place.
	Either in file or args.

	PREVIUS_LEXER is a pointer to the address of previus
	one of double array of t_lexer (&lexer[i - 1])
	
	CURRENT_LEXER is a pointer to the address of current
	one of double array of t_lexer (&lexer[i])
*/
void	move_cmd_to_right_place_if_speacific_tokenid(
		t_lexer **previus_lexer, t_lexer **current_lexer, int i)
{
	if (i > 0)
	{
		if ((*previus_lexer)->tokenid[0]
			== '<' && (*previus_lexer)->tokenid[1] == '\0')
			move_file_name_to_file_and_comand_back(
				(*previus_lexer), (*current_lexer));
		if ((*previus_lexer)->tokenid[0]
			== '<' && (*previus_lexer)->tokenid[1] == '<')
		{
			(*previus_lexer)->args = (*current_lexer)->cmd;
			(*current_lexer)->cmd = NULL;
		}
		if ((*previus_lexer)->tokenid[0] == '>')
		{
			(*previus_lexer)->file = (*current_lexer)->cmd;
			(*current_lexer)->cmd = NULL;
		}
	}
}

/*
	Gets the string user input and seperates all info of it into
	the struct t_lexer for to then be executated
	
	STR is the user input
*/
t_lexer	**main_parser(char *str)
{
	int		token_num;
	int		curser;
	int		i;
	t_lexer	**lexer;

	token_num = count_token(str);
	lexer = (t_lexer **)malloc(sizeof(t_lexer *) * (token_num + 2));
	if (!lexer)
		exit(1);
	i = 0;
	curser = 0;
	while (i <= token_num)
	{
		initiate_values_to_zero_null(&lexer[i], i);
		curser = parse_until_token_id(str, i, lexer, curser);
		if (ft_char_find(lexer[i]->args, "-") != -1)
			move_flags_from_args_to_flags(lexer[i]);
		move_cmd_to_right_place_if_speacific_tokenid(
			&lexer[i - 1], &lexer[i], i);
		removes_front_and_back_redondant_spaces(&lexer[i]);
		i++;
	}
	lexer[i] = NULL;
	i = 0;
	return (lexer);
}

// count num of dash and flags char len
void	count_num_dash_and_flags_char_len(char **str,
		int i, int *num_of_dash, int *flags_char_len)
{
	int	j;

	while ((*str)[i] != '\0')
	{
		if (ft_isspace((*str)[i]) == true || i == 0)
		{
			if (ft_isspace((*str)[i]))
				i++;
			if ((*str)[i] == '\0')
				break ;
			if ((*str)[i] == '-')
			{
				(*num_of_dash)++;
				j = 0;
				while ((*str)[i + j] != '\0' &&
					ft_isspace((*str)[i + j]) == false)
					j++;
				i = i + j;
				(*flags_char_len) = (*flags_char_len) + j;
				continue ;
			}
		}
		i++;
	}
}

//initiate the values in cosponding with num of dash and flags char len
char	*init_new_flag_str(int flags_char_len, char **str, int num_of_dash)
{
	char	*new_str;

	if (ft_strlen((*str)) - (sizeof(char) * (
				flags_char_len + num_of_dash)) != 0)
	{
		new_str = malloc(ft_strlen((*str)) - (sizeof(char)
					* (flags_char_len + num_of_dash)) + 1);
		if (!new_str)
			exit(1);
	}
	else
		new_str = NULL;
	return (new_str);
}

void	set_value_of_flag_str_p2(char **new_str, char **str, int *k, int *i)
{
	(*new_str)[(*k)] = (*str)[(*i)];
	if ((*str)[(*i) + 1] != '\0')
		if (((*str)[(*i) + 1]))
			(*k)++;
	(*i)++;
}

void	place_flag(int *i, char **str, char **flags, int *flags_char_len)
{
	int	j;

	j = 0;
	(*i)++;
	while ((*str)[(*i) + j] != '\0' && ft_isspace((*str)[(*i) + j]) == false)
	{
		(*flags)[(*flags_char_len)] = (*str)[(*i) + j];
		j++;
		(*flags_char_len)++;
	}
	(*i) = (*i) + j;
}

// set the values of string, but doesn't add the '-'
int	set_value_of_flag_str(int *flags_char_len,
		char **str, char	**flags, char **new_str)
{
	int	i;
	int	k;

	i = 0;
	(*flags_char_len) = 0;
	k = 0;
	while ((*str)[i] != '\0')
	{
		if (ft_isspace((*str)[i]) == true || i == 0)
		{
			if (ft_isspace((*str)[i]))
				i++;
			if ((*str)[i] == '-')
			{
				place_flag(&i, str, flags, flags_char_len);
				continue ;
			}
			if ((*str)[i] == '\0')
				break ;
		}
		set_value_of_flag_str_p2(new_str, str, &k, &i);
	}
	return (k);
}

// null terminate all right string or make them null if they are.
void	null_terminate_flags_str(int flags_char_len,
		char **new_str, char **str, char **flags)
{
	int	k;

	k = set_value_of_flag_str(&flags_char_len, str, flags, new_str);
	if (k == 0)
	{
		free((*new_str));
		(*new_str) = NULL;
	}
	if ((*new_str) != NULL)
		(*new_str)[k] = '\0';
	free((*str));
	(*str) = (*new_str);
	(*flags)[flags_char_len] = '\0';
}

char	*get_flags_str(char **str)
{
	char	*flags;
	char	*new_str;
	int		flags_char_len;
	int		i;
	int		num_of_dash;

	if (str == NULL)
		return (NULL);
	i = 0;
	flags_char_len = 0;
	num_of_dash = 0;
	new_str = NULL;
	count_num_dash_and_flags_char_len(str, i, &num_of_dash, &flags_char_len);
	if (flags_char_len == 0)
		return (NULL);
	flags = malloc(sizeof(char) * (flags_char_len + 1));
	if (!flags)
		exit (1);
	new_str = init_new_flag_str(flags_char_len, str, num_of_dash);
	null_terminate_flags_str(flags_char_len, &new_str, str, &flags);
	return (flags);
}
// ! this is for lexer_quotes

char	*sjoin_fr(char const *s1, char const *s2);
char	*handle_expand_doll(char *str);

bool	is_a_token_id(char *input);
int		copy_until_tokenid(int i, char *input_after_curser, char **destination);
int		copy_until_space(int i, char *input_after_curser, char **destination);
int		write_the_right_token(int i, char *input_after_curser, char tokenid[3]);

void	parse_quote_cmd(t_post_quotes **content,
		t_lexer ***lexer, bool *function_done, int i)
{
	if ((*content)->have_to_expand)
		(*lexer)[i]->cmd = handle_expand_doll((*content)->content);
	else
		(*lexer)[i]->cmd = ft_strdup((*content)->content);
	(*function_done) = true;
}

/*
depending on the current situation place
the content of the quotes in the right place
*/
void	parse_with_quotes_quotes_assigning(t_post_quotes **content,
		bool *function_done, t_lexer ***lexer, int i)
{
	if ((*function_done) == false)
		parse_quote_cmd(content, lexer, function_done, i);
	else
	{
		if ((*content)->content[0] == '-')
		{
			if ((*content)->have_to_expand)
				(*lexer)[i]->flags = sjoin_fr((*lexer)[i]->flags,
						handle_expand_doll(&(*content)->content[1]));
			else
				(*lexer)[i]->flags = sjoin_fr((*lexer)[i]->flags,
						ft_strdup(&(*content)->content[1]));
		}
		else
		{
			if ((*content)->have_to_expand)
				(*lexer)[i]->args = sjoin_fr((*lexer)[i]->args,
						sjoin_fr(handle_expand_doll((*content)->content),
							ft_strdup(" ")));
			else
				(*lexer)[i]->args = sjoin_fr((*lexer)[i]->args, sjoin_fr(
							ft_strdup((*content)->content), ft_strdup(" ")));
		}
	}
}

// not sure if done right
void	move_flags_for_quotes(char **curent_content,
		int *j, int i, t_lexer ***lexer)
{
	char	*temp;
	char	*temp2;

	temp = NULL;
	(*j) = copy_until_tokenid(*j, (*curent_content), &temp);
	if (temp != NULL)
	{
		printf("temp == (%s)\n", temp);
		temp2 = get_flags_str(&temp);
		(*lexer)[i]->args = sjoin_fr((*lexer)[i]->args,
				sjoin_fr(temp, ft_strdup(" ")));
		(*lexer)[i]->flags = sjoin_fr((*lexer)[i]->flags, temp2);
	}
}

void	parse_with_quotes_none_quotes_assigning_if_token(char
		**curent_content, int *j, t_lexer ***lexer, int *i)
{
	write_the_right_token((*j), (*curent_content), ((*lexer)[(*i)]->tokenid));
	(*j) = (*j) + ft_strlen((*lexer)[(*i)]->tokenid);
	if ((*lexer)[(*i)]->flags[1] == '\0')
	{
		free((*lexer)[(*i)]->flags);
		(*lexer)[(*i)]->flags = NULL;
	}
	if ((*lexer)[(*i)]->args != NULL)
	{
		(*lexer)[(*i)]->args = remove_front_spaces((*lexer)[(*i)]->args);
		(*lexer)[(*i)]->args = remove_back_spaces((*lexer)[(*i)]->args);
	}
	if ((*lexer)[(*i)]->cmd != NULL)
		(*lexer)[(*i)]->cmd = remove_back_spaces((*lexer)[(*i)]->cmd);
	(*i)++;
	if ((*curent_content)[(*j)] != '\0')
	{
		initiate_values_to_zero_null(&(*lexer)[(*i)], (*i));
		(*lexer)[(*i)]->flags = ft_strdup("-");
		if (!(*lexer)[(*i)]->flags)
			exit(1);
	}
}

void	parse_with_quotes_none_quotes_assigning(char
		**curent_content, bool *function_done, t_lexer ***lexer, int *i)
{
	int	j;

	j = 0;
	while ((*curent_content)[j] != '\0')
	{
		if ((*function_done) == false
			&& ft_isspace((*curent_content)[j]) == false)
		{
			j = copy_until_space(j, (*curent_content), &(*lexer)[(*i)]->cmd);
			(*function_done) = true;
		}
		if (is_a_token_id(&(*curent_content)[j]) == true)
		{
			parse_with_quotes_none_quotes_assigning_if_token(
				curent_content, &j, lexer, i);
			(*function_done) = false;
		}
		if ((*function_done) && ft_isspace((*curent_content)[j]) == false)
		{
			move_flags_for_quotes(curent_content, &j, (*i), &(*lexer));
		}
		if (ft_isspace((*curent_content)[j]))
			j++;
	}
}

// malloc the content
void	parse_with_quotes_init(t_post_quotes **content,
		int	*token_num, t_lexer ***lexer)
{
	int	i;

	i = 0;
	(*token_num) = 0;
	while (content[i] != NULL)
	{
		if (content[i]->is_quotes == false)
			(*token_num) = (*token_num) + count_token(content[i]->content);
		i++;
	}
	(*lexer) = (t_lexer **)malloc(sizeof(t_lexer *) * ((*token_num) + 2));
	if (!(*lexer))
		exit(1);
}

void	parser_with_quotes_p2(t_post_quotes **content,
		bool *function_done, t_lexer ***lexer, int *i)
{
	int	i_content;

	i_content = 0;
	while (content[i_content] != NULL)
	{
		if (content[i_content]->is_quotes)
			parse_with_quotes_quotes_assigning(
				&content[i_content++], function_done, lexer, (*i));
		else
			parse_with_quotes_none_quotes_assigning(
				&content[i_content++]->content, function_done, lexer, i);
	}
	if ((*lexer)[(*i)]->args != NULL)
	{
		(*lexer)[(*i)]->args = remove_front_spaces((*lexer)[(*i)]->args);
		(*lexer)[(*i)]->args = remove_back_spaces((*lexer)[(*i)]->args);
	}
	if ((*lexer)[(*i)]->cmd != NULL)
		(*lexer)[(*i)]->cmd = remove_back_spaces((*lexer)[(*i)]->cmd);
	if ((*lexer)[(*i)]->flags[1] == '\0')
	{
		free((*lexer)[(*i)]->flags);
		(*lexer)[(*i)]->flags = NULL;
	}
}

/*
	Main parser when there are quotes
	take the content of input, and put the info in the right parts of a lexer.
	return that lexeer
*/
t_lexer	**parser_with_quotes(t_post_quotes **content)
{
	t_lexer	**lexer;
	int		token_num;
	int		i;
	bool	function_done;

	parse_with_quotes_init(content, &token_num, &lexer);
	i = 0;
	function_done = false;
	if (function_done == false)
	{
		initiate_values_to_zero_null(&lexer[i], i);
		lexer[i]->flags = ft_strdup("-");
		if (!lexer[i]->flags)
			exit(1);
	}
	parser_with_quotes_p2(content, &function_done, &lexer, &i);
	i++;
	lexer[i] = NULL;
	return (lexer);
}

int		count_char_until_next_token(char *input);

/*
	Return true if INPUT is of these:
	tokenid: ||, |, <<, <, >>, >
*/
bool	is_a_token_id(char *input)
{
	char	**list_of_tokenid;
	int		i;
	int		j;

	list_of_tokenid = get_list_of_tokenid();
	i = 0;
	while (list_of_tokenid[i] != NULL)
	{
		j = 0;
		while (input[j] != '\0' && list_of_tokenid[i][j] == input[j])
		{
			j++;
		}
		if (list_of_tokenid[i][j] == '\0')
		{
			free_double_array(list_of_tokenid);
			return (true);
		}
		i++;
	}
	free_double_array(list_of_tokenid);
	return (false);
}

/*
Copies from index I from INPUT_AFTER_CURSE
into pointer to the string DESTINATION
But only copies until the end of the string or
until a space or until a tokenid.
And return the index at whitch it stoped.
*/
int	copy_until_space(int i, char *input_after_curser, char **destination)
{
	int	j;

	j = 0;
	while (input_after_curser[i + j] != '\0'
		&& (!ft_isspace(input_after_curser[i + j])
			&& !is_a_token_id(&input_after_curser[i + j])))
	{
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

/*
	Copies from index I from INPUT_AFTER_CURSER
	into pointer to the string DESTINATION
	But only copies until the end of the string
	or until a tokenid.
	And return the index at whitch it stoped.
*/
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

/*
	write in TOKENID, the token id curently at INPUT_AFTER_CURSER[I]
	tokenid: ||, |, <<, <, >>, >
*/
int	write_the_right_token(int i, char *input_after_curser, char tokenid[3])
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

/*
	Puts the right values in the LEXER at index CURRENT_LEX, with the string INPUT.
	And continue reading the INPUT starting at the curser.
	and return the new Curser (index at whitch it is done reading)
*/
int	parse_until_token_id(
		char *input, int current_lex, t_lexer **lexer, int curser)
{
	int		i;
	bool	function_done;

	i = 0;
	function_done = false;
	while (input[i + curser] != '\0')
	{
		if (function_done == false && ft_isspace(input[i + curser]) == false)
		{
			i = copy_until_space(i, &input[curser], &lexer[current_lex]->cmd);
			function_done = true;
		}
		if (function_done && ft_isspace(input[i + curser]) == false)
			i = copy_until_tokenid(i,
					&input[curser], &(lexer[current_lex]->args));
		if (is_a_token_id(&input[i + curser]) == true)
		{
			i = write_the_right_token(i,
					&input[curser], (lexer[current_lex]->tokenid));
			break ;
		}
		if (ft_isspace(input[i + curser]))
			i++;
	}
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
				k++;
			if (list_of_tokenid[j][k] == '\0')
			{
				free_double_array(list_of_tokenid);
				return (i);
			}
			j++;
		}
		i++;
	}
	return (free_double_array(list_of_tokenid), i);
}

int	set_k_for_count_token(char *input, int i, char	**list_of_tokenid, int j)
{
	int		k;

	k = 0;
	while (input[i + k] != '\0' && list_of_tokenid[j][k] == input[i + k])
		k++;
	return (k);
}

/*
	Goes threw string input and return the number of tokenID
	tokenid: ||, |, <<, <, >>, >

	PS: I think this is lower then n^2 complexity, log(n) I think
*/
int	count_token(char *input)
{
	char	**list_of_tokenid;
	int		num_of_token;
	int		i;
	int		j;
	int		k;

	list_of_tokenid = get_list_of_tokenid();
	num_of_token = 0;
	i = -1;
	while (input[++i] != '\0')
	{
		j = 0;
		while (list_of_tokenid[j] != NULL)
		{
			k = set_k_for_count_token(input, i, list_of_tokenid, j);
			if (list_of_tokenid[j][k] == '\0')
			{
				i = i + k;
				num_of_token++;
				break ;
			}
			j++;
		}
	}
	return (free_double_array(list_of_tokenid), num_of_token);
}
