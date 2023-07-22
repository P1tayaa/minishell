/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sboulain <sboulain@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 13:30:47 by sboulain          #+#    #+#             */
/*   Updated: 2023/07/22 17:29:34 by sboulain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	maybe add
	include parser.h
*/

// typedef struct s_lexer
// {
// 	char tokenid[3];
// 	char *content;
// 	int	possition;
	
// }	t_lexer;

typedef struct s_lexer
{
    char tokenid[3];
	char *content;
    char *cmd;
    char *flags;
    char *args;
    // char *content;
    int    possition;
    
}    t_lexer;

int		count_pipes(char *str);
char	**get_list_of_tokenid(void);
void	free_list_of_tokenid(char **list_of_tokenid);
int		count_token(char *input);
int	copy_input_to_parcer_and_keep_track_of_curser(char *input, t_lexer *current_lex, int curser);
void	free_parser(t_lexer **lexer);

int parse_until_token_id(char *input, int current_lex, t_lexer **lexer, int curser);


// figure out where ls -la | grep "." | cat > out -e > out2
// the -e should be

t_data_table	*parsse_things(char *str)
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
		printf("mallocing tokennum %d\n", i);
		lexer[i] = (t_lexer *)malloc(sizeof(t_lexer));
		lexer[i]->possition = i;
		// lexer[i]->args = NULL;
		// lexer[i]->cmd = NULL;
		lexer[i]->flags = NULL;
		lexer[i]->tokenid[0] = '\0';
		// curser = copy_input_to_parcer_and_keep_track_of_curser(str, lexer[i], curser);
		curser = parse_until_token_id(str, i, lexer, curser);
		// if (i == 0)
		// 	lexer[i]->tokenid[0] = '\0';
		// else
			
		i++;
	}
	lexer[i] = NULL;
	// printf("lexer[i] == %p\ni == %d, token_num == %d\n", lexer[i], i, token_num);
	i = 0;
	while (lexer[i] != NULL)
	{
		puts("test start reading lexers");
		printf("flags: \"%s\", ",lexer[i]->flags);
		printf("args: \"%s\", ",lexer[i]->args);
		printf("possition %d, ",  lexer[i]->possition);
		printf("cmd: \"%s\", ", lexer[i]->cmd);
		printf("tokenID: \"%s\"\n",lexer[i]->tokenid);
		i++;
	}
	// puts("freeur");
	// free_parser(lexer);
	// puts("end freeur");
	
	return (NULL);

	// * old stuff
	// int				i;
	// t_data_table	*table;
	// 
	// table = malloc(sizeof(t_data_table *));
	// table->number_pip = count_pipes(str);
	// table->list_of_commands = ft_split(str, '|');
	// i = 0;
	// while (table->list_of_commands[i] != NULL)
	// {
	// 	printf("%s\n", table->list_of_commands[i]);
	// 	i++;
	// }
	return (NULL);
}

int	count_char_until_next_token(char *input);
bool	ft_isspace(unsigned char c);

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
		while (list_of_tokenid[i][j] == input[j])
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

int copy_until_space(int i, char *input_after_curser, char **destination)
{
	int	j;

	j = 0;
	// puts("test new");
	while (input_after_curser[i + j] != '\0')
	{
		if ((ft_isspace(input_after_curser[i + j]) || is_a_token_id(&input_after_curser[i + j])))
			break ;
		// puts("test");
		printf("char index %d, is %c\n", j, input_after_curser[i + j]);
		j++;
	}
	(*destination) = (char *)malloc(sizeof(char) * (j + 2));
	if (!(*destination))
		exit(1);
	j = 0;
	while (input_after_curser[i + j] != '\0')
	{
		if ((ft_isspace(input_after_curser[i + j]) || is_a_token_id(&input_after_curser[i + j])))
			break ;
		(*destination)[j] = input_after_curser[i + j];
		j++;
	}
	(*destination)[j] = '\0';
	return (i + j);
}

int copy_until_tokenid(int i, char *input_after_curser, char **destination)
{
	int	j;

	j = 0;
	// puts("test new");
	while (input_after_curser[i + j] != '\0')
	{
		if (is_a_token_id(&input_after_curser[i + j]))
			break ;
		// puts("test");
		printf("char index %d, is %c, next is null %d\n", i + j, input_after_curser[i + j], input_after_curser[i + j + 1] == '\0');
		j++;
	}
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
	// puts("test");
	return (i + j);
}

int	write_the_right_token(int i, char *input_after_curser,char tokenid[3])
{
	int	j;

	j = 0;
	while (input_after_curser[i + j] != '\0' && !ft_isspace(input_after_curser[i + j]) && is_a_token_id(&input_after_curser[i + j]))
	{
		(tokenid)[j] = input_after_curser[i + j];
		j++;
	}
	(tokenid)[j] = '\0';
	return (i + j);
}

int parse_until_token_id(char *input, int current_lex, t_lexer **lexer, int curser)
{
	int i;
	bool function_done;

	puts("test");
	i = 0;
	function_done = false;
	printf("input is %s, curent char %c\n\n", &input[i + curser], input[i + curser]);
	while (input[i + curser] != '\0')
	{
		printf("doing char %d\n", i);
		if (input[i + curser] == '-')
			i = copy_until_space(i, &input[curser], &lexer[current_lex]->flags);
		if (function_done == false && ft_isspace(input[i + curser]) == false)
		{
			puts("saving cmd");
			i = copy_until_space(i, &input[curser], &lexer[current_lex]->cmd);
			// puts("saved cmd");
			puts(lexer[current_lex]->cmd);
			function_done = true;
			continue ;
		}
		if (function_done && ft_isspace(input[i + curser]) == false)
		{
			puts("function_done");
			i = copy_until_tokenid(i, &input[curser], &lexer[current_lex]->args);
		}
		if (is_a_token_id(&input[i + curser]))
		{
			i = write_the_right_token(i, &input[curser], (lexer[current_lex]->tokenid));
			break ;
		}
		
		// printf("did parse %d\n", current_lex);
		i++;
		usleep(10000);
		// printf("doing char %d, and next coninue %d\n", i, input[i + curser] != '\0');
		// pause();
	}
	// puts("test");
	return (curser + i);
}

int	copy_input_to_parcer_and_keep_track_of_curser(char *input, t_lexer *current_lex, int curser)
{
	int		i;
	int		j;
	int		k;
	char	**list_of_tokenid;

	list_of_tokenid = get_list_of_tokenid();
	i = 0;
	current_lex->content = malloc(sizeof(char) * (count_char_until_next_token(&input[curser]) + 1));
	// printf("content size is %d and curser is %d\n", count_char_until_next_token(&input[curser]), curser);
	if (!current_lex->content)
		exit(1);
	while (input[curser + i] != '\0')
	{
		j = 0;
		while (list_of_tokenid[j] != NULL)
		{
			k = 0;
			while (list_of_tokenid[j][k] == input[curser + i + k])
			{
				k++;
			}
			if (list_of_tokenid[j][k] == '\0')
			{
				// puts("did it \n");
				k = 0;
				while (list_of_tokenid[j][k] != '\0')
				{
					current_lex->tokenid[k] = list_of_tokenid[j][k];
					k++;
				}
				current_lex->tokenid[k] = '\0';
				current_lex->content[i] = '\0';
				free_list_of_tokenid(list_of_tokenid);
				return (i + curser + k);
			}
			j++;
		}
		current_lex->content[i] = input[curser + i];
		i++;
	}
	free_list_of_tokenid(list_of_tokenid);
	current_lex->tokenid[0] = '\0';
	current_lex->content[i] = '\0';
	return(i + curser);
}

void	free_parser(t_lexer **lexer)
{
	int	i;

	if (!lexer)
		return ;
	i = 0;
	while (lexer[i] != NULL)
	{
		// printf("address of lexer[i] == %p\nfreeing lexer %d content\n", lexer[i], i);
		free(lexer[i]->content);
		// printf("freeing lexer %d branch\n", i);
		free(lexer[i]);
		i++;
	}
	// printf("freeing lexer\n");
	free(lexer);
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
	return(i);
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
			while (list_of_tokenid[j][k] == input[i + k])
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

int	count_pipes(char *str)
{
	int	i;
	int	number_of_pipes;

	i = 0;
	number_of_pipes = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '|')
			number_of_pipes++;
		i++;
	}
	return (number_of_pipes);
}

char **get_list_of_tokenid(void)
{
	char **list_of_tokenid;

	list_of_tokenid = (char **)malloc(sizeof(char *) * 7);
	list_of_tokenid[0] = ft_strdup("||");
	list_of_tokenid[1] = ft_strdup("|");
	list_of_tokenid[2] = ft_strdup("<<");
	list_of_tokenid[3] = ft_strdup("<");
	list_of_tokenid[4] = ft_strdup(">>");
	list_of_tokenid[5] = ft_strdup(">");
	list_of_tokenid[6] = NULL;
	return (list_of_tokenid);
}

void	free_list_of_tokenid(char **list_of_tokenid)
{
	int	i;

	i = 0;
	while (list_of_tokenid[i] != NULL)
	{
		free(list_of_tokenid[i]);
		i++;
	}
	if (list_of_tokenid)
		free(list_of_tokenid);
}
