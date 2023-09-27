/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sboulain <sboulain@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 13:30:47 by sboulain          #+#    #+#             */
/*   Updated: 2023/09/27 16:42:14 by sboulain         ###   ########.fr       */
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

/*
	Return the number of charaters that are going to be moved from arg to flags,
	And keeps track of how many are going to be left in arg with ARGS_CHAR_LEN
*/
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

/*
	Malloc in new args and flags with the pointer to the address given in ARGS and FLAGS
*/
void	malloc_new_args_and_new_flags(int args_char_len, char **args, char **flags, int flags_char_len)
{
	if (args_char_len != 0)
	{
		(*args) = malloc(sizeof(char) * (args_char_len + 1));
		if (!(*args))
			exit (-1);
	}
	else
		(*args) = NULL;
	(*flags) = malloc(sizeof(char) * (flags_char_len + 1));
	if (!(*flags))
		exit (-1);
}

/*
	Move the flags from args to there respective par of the lexer by reconising where the '-'
	
*/
void	move_flags_from_args_to_flags(t_lexer *lexer)
{
	int	i;
	char *args;
	char *flags;
	int flags_char_len;
	int args_char_len;

	// count how many char are to be in flags and args
	flags_char_len = count_number_flags_char(&args_char_len, lexer);

	// malloc for size
	malloc_new_args_and_new_flags(args_char_len, &args, &flags, flags_char_len);

	// copy in them
	
	flags[0] = '-';
	flags_char_len = 1;
	args_char_len = 0;
	i = 0;
	while (lexer->args[i] != '\0')
	{
		if (i != 0)
		{
			// printf("bool status %d", ft_isspace(lexer->args[i - 1]) && lexer->args[i] == '-');
			if (ft_isspace(lexer->args[i - 1]) && lexer->args[i] == '-')
			{
				i++;
				while (!ft_isspace(lexer->args[i]) && lexer->args[i] != '\0')
				{
					flags[flags_char_len] = lexer->args[i];
					flags_char_len++;
					i++;
				}
				// flags[flags_char_len] = ' ';
				// flags_char_len++;
				continue ;
			}
		}
		if (i == 0 && lexer->args[i] == '-')
		{
			i++;
			while (!ft_isspace(lexer->args[i]) && lexer->args[i] != '\0')
			{
				printf("added char to flags: %c\n", lexer->args[i]);
				flags[flags_char_len] = lexer->args[i];
				flags_char_len++;
				i++;
			}
			// flags[flags_char_len] = ' ';
			// flags_char_len++;
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


/*
	Put the cmd from current lexer into file of the previus lexer.
	Than moves the first word from agrs back to cmd, in case cmd was actually a file.
*/
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

/*
	Return a string with all the characters at the fronts that is a space.
	It frees the origial STR variable
*/
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

/*
	Return a string with all the characters at the back that is a space.
	It frees the origial STR variable
*/
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

/*
	Initiates values to zero or null and malloc the new lexer i.
	
	LEXER is a pointer to the address of current one of double array of t_lexer (&lexer[i])
*/
void	initiate_values_to_zero_NULL(t_lexer **lexer, int i)
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
}

/*
	Removes the spaces that are at the front and back of all parts of LEXER
	by using remove_front_spaces() and remove_back_spaces()
	
	LEXER is a pointer to the address of current one of double array of t_lexer (&lexer[i])
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

	PREVIUS_LEXER is a pointer to the address of previus one of double array of t_lexer (&lexer[i - 1])
	CURRENT_LEXER is a pointer to the address of current one of double array of t_lexer (&lexer[i])
*/
void	move_cmd_to_right_place_if_speacific_tokenid(t_lexer **previus_lexer, t_lexer **current_lexer, int i)
{
	if (i > 0)
	{
		if ((*previus_lexer)->tokenid[0] == '<' && (*previus_lexer)->tokenid[1] == '\0')
			move_file_name_to_file_and_comand_back((*previus_lexer), (*current_lexer));
		if ((*previus_lexer)->tokenid[0] == '<' && (*previus_lexer)->tokenid[1] == '<')
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
		initiate_values_to_zero_NULL(&lexer[i], i);
		curser = parse_until_token_id(str, i, lexer, curser);
		if (ft_char_find(lexer[i]->args, "-") != -1)
		{
			move_flags_from_args_to_flags(lexer[i]);
		}
		move_cmd_to_right_place_if_speacific_tokenid(&lexer[i - 1], &lexer[i], i);
		removes_front_and_back_redondant_spaces(&lexer[i]);
		i++;
	}
	lexer[i] = NULL;
	i = 0;
	return (lexer);
}

char *get_flags_str(char **str)
{
	char	*flags;
	char	*new_str;
	int		flags_char_len;
	int		i;
	int		j;
	int		k;
	int		num_of_dash;

	// puts("get_flags_str starts");
	i = 0;
	flags_char_len = 0;
	num_of_dash = 0;
	// printf("the is str is (%s)\n", *str);
	while ((*str)[i] != '\0')
	{
		// printf("i == %d, curent char is %c\n", i, (*str)[i]);
		if (ft_isspace((*str)[i]) == true || i == 0)
		{
			// puts("is space");
			if (ft_isspace((*str)[i]))
				i++;
			if ((*str)[i] == '-')
			{
				// puts("find flag");
				num_of_dash++;
				j = 0;
				while ((*str)[i + j] != '\0' && ft_isspace((*str)[i + j]) == false)
				{
					j++;
				}
				i = i + j;
				flags_char_len = flags_char_len + j;
				continue ;
			}
			if ((*str)[i] == '\0')
				break ;
		}
		i++;
	}
	// printf("the is flags len is (%d)\n", flags_char_len);
	if (flags_char_len == 0)
		return (NULL);
	flags = malloc(sizeof(char) * (flags_char_len + 1));
	if (!flags)
		exit (1);
	if (ft_strlen((*str)) - (sizeof(char) * (flags_char_len + num_of_dash)) != 0)
	{
		new_str = malloc(ft_strlen((*str)) - (sizeof(char) * (flags_char_len + num_of_dash)) + 1);
		if (!new_str)
			exit(1);
	}
	else
		new_str = NULL;
	i = 0;
	flags_char_len = 0;
	k = 0;
	// printf("the is str is (%s)\n", str);
	while ((*str)[i] != '\0')
	{
		// printf("curent char is (%c)\n", str[i]);
		if (ft_isspace((*str)[i]) == true || i == 0)
		{
			if (ft_isspace((*str)[i]))
				i++;
			if ((*str)[i] == '-')
			{
				j = 0;
				i++;
				while ((*str)[i + j] != '\0' && ft_isspace((*str)[i + j]) == false)
				{
					// printf("char flags is: %c\n", str[i + j]);
					flags[flags_char_len] = (*str)[i + j];
					j++;
					flags_char_len++;
				}
				i = i + j;
				// printf("i == %d, char is %c\n", i, str[i]);
				continue ;
			}
			if ((*str)[i] == '\0')
				break ;
		}
		// printf("k == %d, char with %c\n", k, str[i]);
		new_str[k] = (*str)[i];
		k++;
		i++;
	}
	// puts("end flags");
	// printf("k == %d\n", k);
	if (k == 0 && new_str != NULL)
	{
		free(new_str);
		new_str = NULL;
	}
	if (new_str != NULL)
		new_str[k] = '\0';
	free((*str));
	(*str) = new_str;
	// puts(*str);
	flags[flags_char_len] = '\0';
	return (flags);
}

char	*ft_strjoin_with_frees(char const *s1, char const *s2);
char	*handle_expand_doll(char *str);

bool	is_a_token_id(char *input);
int	copy_until_tokenid(int i, char *input_after_curser, char **destination);
int	copy_until_space(int i, char *input_after_curser, char **destination);
int	write_the_right_token(int i, char *input_after_curser,char tokenid[3]);

t_lexer	**parser_with_quotes(t_post_quotes **content)
{
	t_lexer **lexer;
	int	token_num;
	int i;

	// malloc the content
	i = 0;
	token_num = 0;
	while (content[i] != NULL)
	{
		if (content[i]->is_quotes == false)
			token_num = token_num + count_token(content[i]->content);
		i++;
	}
	lexer = (t_lexer **)malloc(sizeof(t_lexer *) * (token_num + 2));
	if (!lexer)
		exit(1);

	int i_content;
	bool function_done;
	// fill the content
	i = 0;
	i_content = 0;
	function_done = false;
	while (token_num >= i)
	{
		if (function_done == false)
		{
			initiate_values_to_zero_NULL(&lexer[i], i);
			lexer[i]->flags = ft_strdup("-");
		}
		while (content[i_content] != NULL)
		{
			if (content[i_content]->is_quotes)
			{
				if (function_done == false)
				{
					if (content[i_content]->have_to_expand)
						lexer[i]->cmd = handle_expand_doll(content[i_content]->content);
					else
						lexer[i]->cmd = ft_strdup(content[i_content]->content);
					function_done = true;
				}
				else
				{
					if (content[i_content]->content[0] == '-')
						if (content[i_content]->have_to_expand)
							lexer[i]->flags = ft_strjoin_with_frees(lexer[i]->flags, handle_expand_doll(&content[i_content]->content[1]));
						else
							lexer[i]->flags = ft_strjoin_with_frees(lexer[i]->flags, ft_strdup(&content[i_content]->content[1]));
					else
						if (content[i_content]->have_to_expand)
							lexer[i]->args = ft_strjoin_with_frees(lexer[i]->args, ft_strjoin_with_frees(handle_expand_doll(content[i_content]->content), ft_strdup(" ")));
						else
							lexer[i]->args = ft_strjoin_with_frees(lexer[i]->args, ft_strjoin_with_frees(ft_strdup(content[i_content]->content), ft_strdup(" ")));
				}
				i_content++;
			}
			else
			{	
				int j;
				
				j = 0;
				while (content[i_content]->content[j] != '\0')
				{
					if (function_done == false && ft_isspace(content[i_content]->content[j]) == false)
					{
						// puts
						j = copy_until_space(j, content[i_content]->content, &lexer[i]->cmd);
						function_done = true;
						// continue ;
					}
					if (function_done && ft_isspace(content[i_content]->content[j]) == false)
					{
						char *temp;
						char *temp2;

						j = copy_until_tokenid(j, content[i_content]->content, &temp);
						// : need to add a check for flags here that moves it to flags
						temp2 = get_flags_str(&temp);
						// puts(temp2);
						// pause();
						if (temp != NULL)
						{
							// temp = remove_back_spaces(temp);
							lexer[i]->args = ft_strjoin_with_frees(lexer[i]->args, ft_strjoin_with_frees(temp, ft_strdup(" ")));
						}
						lexer[i]->flags = ft_strjoin_with_frees(lexer[i]->flags, temp2);
						// free(content[i_content]->content);
						// content[i_content]->content = temp;
						// j = 0;
						// continue ;
					}
					if (is_a_token_id(&content[i_content]->content[j]) == true)
					{
						// puts("test");
						write_the_right_token(j, content[i_content]->content, (lexer[i]->tokenid));
						j = j + ft_strlen(lexer[i]->tokenid);
						function_done = false;
						if (lexer[i]->flags[1] == '\0')
						{
							free(lexer[i]->flags);
							lexer[i]->flags = NULL;
						}
						i++;
						if (content[i_content]->content[j] != '\0')
						{
							initiate_values_to_zero_NULL(&lexer[i], i);
							lexer[i]->flags = ft_strdup("-");
						}
					}
					if (ft_isspace(content[i_content]->content[j]))
						j++;
					// printf("parse none quotes, j == %d, curent char is %c, is tokenid %d\n", j, content[i_content]->content[j], is_a_token_id(&content[i_content]->content[j]));
					// usleep(10000);
					// printf("parse none quotes, j == %d, curent char is %c, is tokenid %d\n", j, content[i_content]->content[j], is_a_token_id(&content[i_content]->content[j]));

					// pause();
				}
				i_content++;
			}
		}
		if (lexer[i]->args != NULL)
			lexer[i]->args = remove_back_spaces(lexer[i]->args);
		if (lexer[i]->flags[1] == '\0')
		{
			free(lexer[i]->flags);
			lexer[i]->flags = NULL;
		}
		i++;
	}
	lexer[i] = NULL;
	
	i = 0;
	// while (lexer[i] != NULL)
	// {
	// 	printf("\nlexer[%d]\n cmd: (%s)\n", i, lexer[i]->cmd);
	// 	printf("args: (%s)\n", lexer[i]->args);
	// 	printf("tokenid: (%s)\n", lexer[i]->tokenid);
	// 	printf("file: (%s)\n", lexer[i]->file);
	// 	printf("flags: (%s)\n", lexer[i]->flags);
	// 	i++;
	// }
	// pause();
	
	return lexer;
}

int	count_char_until_next_token(char *input);

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
			free_list_of_tokenid(list_of_tokenid);
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
			// continue ;
		}
		if (function_done && ft_isspace(input[i + curser]) == false)
		{
			i = copy_until_tokenid(i, &input[curser], &(lexer[current_lex]->args));
			// continue ;
		}
		if (is_a_token_id(&input[i + curser]) == true)
		{
			i = write_the_right_token(i, &input[curser], (lexer[current_lex]->tokenid));
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
	puts(input);
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
