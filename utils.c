/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 17:01:53 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/10/29 18:10:13 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_char_find(char *str, const char *list_of_char);
bool	ft_isspace(unsigned char c);

char	*ft_strtok(char *str, const char *delim)
{
	int			i;
	int			index_unter_delim;
	static int	curser;
	static char	*str_reminder;
	char	*return_char;

	if (str != NULL)
	{
		curser = 0;
		str_reminder = str;
	}
	if (str_reminder == NULL)
		return (NULL);
	index_unter_delim = ft_char_find(&str_reminder[curser], delim);
	if (index_unter_delim == -1)
	{
		if (str_reminder[curser] == '\0')
			return (NULL);
		index_unter_delim = ft_strlen(&str_reminder[curser]);
	}
	return_char = malloc(sizeof(char) * (index_unter_delim + 1));
	i = 0;
	while (i < index_unter_delim)
	{
		return_char[i] = str_reminder[curser + i];
		i++;
	}
	curser = curser + 1;
	return_char[i] = '\0';
	return (return_char);
}

char *get_string_at_index(char *string, int index)
{
	int	i;

	if (string == NULL)
		return (NULL);
	i = 0;
	while (string[i] != '\0')
	{
		if (i == index)
			return (&string[i]);
		i++;
	}
	return (NULL);
}

int ft_char_find(char *str, const char *list_of_char)
{
	int i;
	int	j;

	if (str == NULL)
		return -1;
	i = 0;
	// puts(str);
	while (str[i] != '\0')
	{
		j = 0;
		while (list_of_char[j] != '\0')
		{
			if (str[i] == list_of_char[j])
				return (i);
			j++;
		}
		i++;
		// printf("\nft_char_find: i == %i\n", i);
	}
	return (-1);
}
// {
// 	static char	*last_token;
// 	int			index;

// 	last_token = NULL;
// 	index = 0;
// 	if (str == NULL)
//         str = last_token;					// Use the last string if NULL is passed
// 	while (str[index] != '\0')
// 	{
// 		if (strchr(delim, str[index]))
// 			break;
// 		index++;
// 	}
// 	if (str[index] == '\0')					// segfault is here, tho ????
// 	{
//         last_token = NULL;
//         return (str);
//     }
// 	str[index] = '\0';
// 	last_token = &str[index + 1];
// 	return (str);
// }

char *str_dup_until_index(char *str, int index)
{
	char *str_dup;
	int i;

	if (index == 0)
		return (ft_strdup("\0"));
	str_dup = (char *)malloc(sizeof(char) * (index + 2));
	if (!str_dup)
		exit(1);
	i = 0;
	while (str[i] != '\0' && i != index)
	{
		str_dup[i] = str[i];
		i++;
	}
	str_dup[i] = '\0';
	return (str_dup);
}

void	concat_path(char *buffer, const char *dir, const char *cmd)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (!cmd)
		return ;
	while (dir[i] != '\0')
	{
		buffer[i] = dir[i];
		i++;
	}
	buffer[i] = '/';
	i++;
	while (cmd[j] != '\0')
	{
		buffer[i] = cmd[j];
		i++;
		j++;
	}
	buffer[i] = '\0';
}
/*
	Joins the S1 and S2 and return it (malloced), and free S1 and S2.
*/
char	*ft_strjoin_with_frees(char *s1, char *s2)
{
	size_t	len;
	char	*ptr;

	if (s1 == NULL)
	{
		ptr = ft_strdup(s2);
		if (s2)
			free(s2);
		return (ptr);
	}
	len = ft_strlen(s1) + ft_strlen(s2);
	ptr = malloc(len + 1);
	if (!ptr)
		return (NULL);
	ft_strlcpy(ptr, s1, len + 1);
	ft_strlcat(ptr, s2, len + 1);
	if (s1)
		free(s1);
	s1 = NULL;
	if (s2)
		free(s2);
	s2 = NULL;
	return (ptr);
}
/*
	take STR_OG (usually the arg), and check if there is $?.
	If it doesn't, it just return STR_OG. 
	Otherwise it make a copy of STR_OG, but replace $? with NUMBER_REPLACE.
	Returning the new str, and freeing STR_OG
*/
char *replace_doll_question_to_number_with_free(char *str_og, int number_replace)
{
	int	i;
	int	location_of_doll;
	char *str_return;

	if (str_og == NULL)
		return NULL;
	i = 0;
	location_of_doll = -1;
	while (str_og[i] != '\0')
	{
		if (str_og[i] == '$')
		{
			if (str_og[i + 1] == '\0')
				break ;
			else if (str_og[i + 1] == '!')
				if (ft_isspace(str_og[i + 2]) || str_og[i + 2] == '\0')
					location_of_doll = i;
		}
		i++;
	}
	if (location_of_doll == -1)
		return (str_og);
	str_return = ft_strjoin_with_frees(str_dup_until_index(str_og, location_of_doll), ft_strjoin_with_frees(ft_itoa(number_replace), ft_strdup(&str_og[location_of_doll + 2])));
	if (!str_return)
		exit(1);
	free(str_og);
	return (str_return);
}

int return_biggest_int(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

char *here_doc_starter(char *wordlocking_for)
{
    // int i;
    char *str_return;
    char *read_line_str;
    
    str_return = NULL;
    read_line_str = readline(" > ");
    while (ft_strncmp(wordlocking_for, read_line_str, return_biggest_int(ft_strlen(wordlocking_for), ft_strlen(read_line_str)) )!= 0)
    {
        str_return = ft_strjoin_with_frees(str_return, ft_strjoin("\n", read_line_str));
        read_line_str = readline(" > ");
    }
	str_return = ft_strjoin_with_frees(str_return, ft_strdup("\n"));
    return (str_return);
}


bool is_str_export(char *str)
{
	if (str[0] == 'e' && str[1] == 'x' && str[2] == 'p'
		&& str[3] == 'o' && str[4] == 'r' && str[5] == 't' 
			&& str[6] == '\0')
			return (true);
	return (false);
}

char	*ft_strdup_until_space(char *str)
{
	int i;

	i = 0;
	while (!ft_isspace(str[i]) && str[i] != '\0')
		i++;
	return(str_dup_until_index(str, i));
}

char	*ft_strdup_until_equal(char *str)
{
	int i;

	i = 0;
	while (str[i] != '=' && str[i] != '\0')
		i++;
	return(str_dup_until_index(str, i));
}

int	get_num_export_con(char *arg)
{
	int	i;
	bool	is_space;
	int		num_var;

	i = 0;
	is_space = false;
	num_var = 1;
	while (arg[i] != '\0')
	{
		if (!is_space && !ft_isspace(arg[i]))
		{
			num_var++;
			is_space = true;
		}
		else
			if (ft_isspace(arg[i]))
				is_space = false;
		i++;
	}
	return (num_var * 2);
}

bool	ft_isspace(unsigned char c);

char **get_word_one_by_one(char *str)
{
	char **all_world;
	// char ***all_world_pointer;
	int i;
	int	i_all_world;

	all_world = malloc(sizeof(char *) * ((get_num_export_con(str) * 2) + 1));
	i = 0;
	i_all_world = 0;
	puts(str);
	while (str[i] != '\0')
	{
		while (ft_isspace(str[i]))
		{
			i++;
		}
		
		all_world[i_all_world] = ft_strdup_until_space(&str[i]);
		if (str[i + ft_strlen(all_world[i_all_world])] == '\0')
		{
			i_all_world++;
			break ;
		}
		i = i + ft_strlen(all_world[i_all_world]) + 1;
		i_all_world++;
	}
	printf("i_all_world = %i\n", i_all_world);
	all_world[i_all_world] = NULL;
	i = 0;
	puts("");
	puts("");

	while (all_world[i] != NULL)
	{
		printf("content i == %i, is (%s)\n", i, all_world[i]);
		i++;
	}
	puts("");
	puts("");
	puts("");
	return (all_world);
}

char **get_export_var(char *arg_of_export)
{
	// char *var_name;
	// char *var_content;
	char **work_split;
	char **export_content;
	int		i_export_content;
	int		i_work_split;

	// char *str_tok_arg;

	i_export_content = 0;
	i_work_split = 0;
	export_content = malloc(sizeof(char *) * ((get_num_export_con(arg_of_export) * 2) + 3));
	work_split = get_word_one_by_one(arg_of_export);
	while (work_split[i_work_split] != NULL)
	{
		i_work_split++;
	}
	i_work_split = 0;
	while (work_split[i_work_split] != NULL)
	{
		// puts(work_split[i_work_split]);
		export_content[i_export_content] = ft_strdup_until_equal(work_split[i_work_split]);
		i_export_content++;
		if (ft_strlen(export_content[i_export_content - 1]) == ft_strlen(work_split[i_work_split]))
			export_content[i_export_content] = ft_strdup("\0");
		else
			export_content[i_export_content] = ft_strdup(&((work_split[i_work_split])[ft_strlen(export_content[i_export_content - 1]) + 1]));
		i_export_content++;
		free(work_split[i_work_split]);
		i_work_split++;
	}
	puts("test finito");
	free(work_split);
	export_content[i_export_content] = NULL;
	return (export_content);
}

void export_content_freeur(char **export_content)
{
	int i;

	i = 0;
	while (export_content[i] != NULL)
	{
		free(export_content[i]);
		i++;
	}
	free(export_content);
}

void	test_export_for_main(t_lexer **lexer)
{
	char **export_content;
	int i;

	if (is_str_export(lexer[0]->args) == true)
	{
		export_content = get_export_var(lexer[0]->args);
		puts("testoep");
		if (export_content == NULL)
			printf("error\n");
		else
		{
			i = 0;
			while (export_content[i] != NULL)
			{
				printf("content i == %i, is (%s)\n", i, export_content[i]);
				i++;
			}
		}
	}
	pause();
}

int	counte_num_new_var(t_post_quotes	**content)
{
	int i;
	int	num_of_vars;

	i = 0;
	num_of_vars = 0;
	while (content[i] != NULL)
	{
		if (content[i]->is_quotes)
		{
			num_of_vars++;
		}
		else
		{
			num_of_vars += get_num_export_con(content[i]->content);
		}
		i++;
	}
	printf("num of var in quotes is : %i\n", num_of_vars);
	return (num_of_vars * 2);
}

bool	is_all_space(char *str)
{
	int i;

	i = 0;
	while (str[i] != '\0')
	{
		if (!ft_isspace(str[i]))
			return (false);
		i++;
	}
	return (true);
}

bool	check_if_array_str_is_empty(char **array_str)
{
	int i;

	if (array_str == NULL)
		return (true);
	i = 0;
	while (array_str[i])
	{
		if (array_str[i][0] != '\0')
			return (false);
		i++;
	}
	return (true);
}

char	get_last_char(char *str)
{
	int i;

	i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	i--;
	return(str[i]);
}

bool	check_export_for_quotes(t_post_quotes	**content)
{
	int 		i;
	int 		j;
	int			i_export_content;
	char 		**export_content;
	char		**temp_var_from_no_quotes;

	if (is_str_export(content[0]->content) == false)
		return (false);
	export_content = malloc(sizeof(char *) * ((counte_num_new_var(content) * 2) + 1));
	i = 1;
	i_export_content = 0;
	while (content[i] != NULL)
	{
		printf("doing content %i, and writing item %d\n", i, i_export_content);
		if (content[i]->is_quotes)
		{
			// if (i % 2 == 0)
			// {
			// 	//name of var
				
			// }
			// else
			// {
			// 	//content of var

			// }
			export_content[i_export_content] = ft_strdup(content[i]->content);
			i_export_content++;
		}
		else
		{
			if (is_all_space(content[i]->content) == false)
			{
				temp_var_from_no_quotes = get_export_var(content[i]->content);
				if (check_if_array_str_is_empty(temp_var_from_no_quotes) == true)
				{
					puts("ALL EMPTY NONE QUOTES");
					export_content_freeur(temp_var_from_no_quotes);
					i++;
					continue ;
				}
				j = 0;
				if (i_export_content % 2 == 1 && content[i]->content[0] == '=')
					j++;
				if (i_export_content % 2 == 1 && content[i]->content[0] != '=')
				{
					export_content[i_export_content] = ft_strdup("");
					i_export_content++;
				}
				printf("j == %i\n", j);
				while (temp_var_from_no_quotes[j] != NULL)
				{
					printf("temp_var is %i == (%s), coping in %i\n", j,temp_var_from_no_quotes[j], i_export_content);
					// if ()
					// if (j != 0)
					// 	if (temp_var_from_no_quotes[j + 1] == NULL && temp_var_from_no_quotes[j][0] == '\0')
					// 		break ;
					export_content[i_export_content] = temp_var_from_no_quotes[j];
					j++;
					i_export_content++;
				}
				free(temp_var_from_no_quotes);
			}
			else if (i_export_content % 2 == 1)
			{
				puts("contentless nonequotes");
				export_content[i_export_content] = ft_strdup("");
				i_export_content++;
			}
			// {

			// }
		}
		i++;
	}
	printf("%d is where null is \n", i_export_content);
	export_content[i_export_content] = NULL;
	i = 0;
	while (export_content[i] != NULL)
	{
		if (i % 2 == 0)
			printf("(%s)", export_content[i]);
		else 
			printf(" = (%s)\n", export_content[i]);
		i++;
	}
	printf("\n");

	pause();
	return (true);
}
