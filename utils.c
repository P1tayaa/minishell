/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sboulain <sboulain@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 17:01:53 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/17 18:18:28 by sboulain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_char_find(char *str, const char *list_of_char);
bool	ft_isspace(unsigned char c);

int	ft_strtok_line_help(
	char **str_reminder, int *index_unter_delim, int *curser)
{
	(*index_unter_delim) = ft_strlen(&(*str_reminder)[(*curser)]);
	return (1);
}

char	*ft_strtok(char *str, const char *delim)
{
	int			i;
	int			index_unter_delim;
	static int	curser;
	static char	*str_reminder;
	char		*return_char;

	if (str != NULL)
	{
		curser = 0;
		str_reminder = str;
	}
	if (str_reminder == NULL)
		return (NULL);
	index_unter_delim = ft_char_find(&str_reminder[curser], delim);
	if (index_unter_delim == -1)
		if (str_reminder[curser] == '\0'
			&& ft_strtok_line_help(&str_reminder, &index_unter_delim, &curser))
			return (NULL);
	return_char = malloc(sizeof(char) * (index_unter_delim + 1));
	i = -1;
	while (++i < index_unter_delim)
		return_char[i] = str_reminder[curser + i];
	curser = curser + 1;
	return_char[i] = '\0';
	return (return_char);
}

char	*get_string_at_index(char *string, int index)
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

int	ft_char_find(char *str, const char *list_of_char)
{
	int	i;
	int	j;

	if (str == NULL)
		return (-1);
	i = 0;
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
	}
	return (-1);
}

char	*str_dup_until_index(char *str, int index)
{
	char	*str_dup;
	int		i;

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

bool	find_doll_question(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '$')
		{
			if (str[i + 1] == '\0')
				return (false);
			if (str[i + 1] == '!')
				return (true);
		}
		i++;
	}
	return (false);
}

char	*replace_doll_question_to_number_with_free(
			char *str_og, int number_replace);

char	*mk_doll_ques_into_num(char **str_og,
	int number_replace, int location_of_doll)
{
	char	*str_return;

	str_return = ft_strjoin_with_frees(str_dup_until_index((*str_og),
				location_of_doll), ft_strjoin_with_frees(
				ft_itoa(number_replace),
				ft_strdup(&(*str_og)[location_of_doll + 2])));
	if (!str_return)
		exit(1);
	if (find_doll_question(str_return))
		str_return = replace_doll_question_to_number_with_free(
				str_return, number_replace);
	return (str_return);
}

/*
	take STR_OG (usually the arg), and check if there is $?.
	If it doesn't, it just return STR_OG. 
	Otherwise it make a copy of STR_OG, but replace $? with NUMBER_REPLACE.
	Returning the new str, and freeing STR_OG
*/
char	*replace_doll_question_to_number_with_free(
		char *str_og, int number_replace)
{
	int		i;
	int		location_of_doll;
	char	*str_return;

	if (g_exit_status != 0)
		number_replace = g_exit_status;
	g_exit_status = 0;
	if (str_og == NULL)
		return (NULL);
	i = -1;
	location_of_doll = -1;
	while (str_og[++i] != '\0')
	{
		if (str_og[i] == '$')
		{
			if (str_og[i + 1] == '\0')
				break ;
			else if (str_og[i + 1] == '!')
				if (ft_isspace(str_og[i + 2]) || str_og[i + 2] == '\0')
					location_of_doll = i;
		}
	}
	if (location_of_doll == -1)
		return (str_og);
	str_return = mk_doll_ques_into_num(&str_og,
			number_replace, location_of_doll);
	return (free(str_og), str_return);
}

int	return_biggest_int(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

char *here_doc_starter(char *wordlocking_for)
{
    char *str_return;
    char *read_line_str;
    
    str_return = NULL;
    read_line_str = readline(" > ");
    while (ft_strncmp(wordlocking_for, read_line_str, return_biggest_int(ft_strlen(wordlocking_for), ft_strlen(read_line_str)))!= 0)
    {
        str_return = ft_strjoin_with_frees(str_return, ft_strjoin("\n", read_line_str));
        read_line_str = readline(" > ");
    }
	str_return = ft_strjoin_with_frees(str_return, ft_strdup("\n"));
	return (str_return);
}


bool	is_str_export(char *str)
{
	if (ft_strlen(str) == 6)
		if (str[0] == 'e' && str[1] == 'x' && str[2] == 'p'
			&& str[3] == 'o' && str[4] == 'r' && str[5] == 't'
			&& str[6] == '\0')
			return (true);
	return (false);
}

bool	is_str_unset(char *str)
{
	if (!str)
		if (ft_strlen(str) != 5)
			return (false);
	if (str[0] == 'u' && str[1] == 'n' && str[2] == 's'
		&& str[3] == 'e' && str[4] == 't' && str[5] == '\0')
		return (true);
	return (false);
}

char	*ft_strdup_until_space(char *str)
{
	int	i;

	i = 0;
	while (!ft_isspace(str[i]) && str[i] != '\0')
		i++;
	return (str_dup_until_index(str, i));
}

char	*ft_strdup_until_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '=' && str[i] != '\0')
		i++;
	return (str_dup_until_index(str, i));
}

int	get_num_export_con(char *arg)
{
	int		i;
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

char	**get_word_one_by_one(char *str)
{
	char	**all_world;
	int		i;
	int		i_all_world;

	all_world = malloc(sizeof(char *) * ((get_num_export_con(str) * 2) + 1));
	i = 0;
	i_all_world = 0;
	while (str[i] != '\0')
	{
		while (ft_isspace(str[i]))
			i++;
		all_world[i_all_world] = ft_strdup_until_space(&str[i]);
		if (str[i + ft_strlen(all_world[i_all_world])] == '\0')
		{
			i_all_world++;
			break ;
		}
		i = i + ft_strlen(all_world[i_all_world]) + 1;
		i_all_world++;
	}
	all_world[i_all_world] = NULL;
	return (all_world);
}

/*
og vars
char	**work_split;
char	**export_content;
int		i_export_content;
int		i_work_split;

*/

char	**get_export_var(char *arg_of_export)
{
	char	**work_split;
	char	**export_content;
	int		i_c;
	int		i_w;

	i_c = 0;
	i_w = 0;
	export_content = malloc(sizeof(char *)
			* ((get_num_export_con(arg_of_export) * 2) + 3));
	work_split = get_word_one_by_one(arg_of_export);
	i_w = 0;
	while (work_split[i_w] != NULL)
	{
		export_content[i_c] = ft_strdup_until_equal(work_split[i_w]);
		i_c++;
		if (ft_strlen(export_content[i_c - 1]) == ft_strlen(work_split[i_w]))
			export_content[i_c] = ft_strdup("\0");
		else
			export_content[i_c] = ft_strdup(&((work_split[i_w])
					[ft_strlen(export_content[i_c - 1]) + 1]));
		i_c++;
		free(work_split[i_w++]);
	}
	export_content[i_c] = NULL;
	return (free(work_split), export_content);
}

void	export_content_freeur(char ***export_content)
{
	int	i;

	i = 0;
	while ((*export_content)[i] != NULL)
	{
		free((*export_content)[i]);
		i++;
	}
	free((*export_content));
}

void	test_export_for_main(t_lexer **lexer)
{
	char	**export_content;
	int		i;

	if (is_str_export(lexer[0]->args) == true)
	{
		export_content = get_export_var(lexer[0]->args);
		if (export_content == NULL)
			printf("error\n");
		else
		{
			i = 0;
			while (export_content[i] != NULL)
				i++;
		}
	}
	pause();
}

int	counte_num_new_var(t_post_quotes **content)
{
	int	i;
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
	return (num_of_vars * 2);
}

bool	is_all_space(char *str)
{
	int	i;

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
	int	i;

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
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	i--;
	return (str[i]);
}

void	set_env(char *name, char *value, char ***environment);
char	***get_env(void);
void	print_export(char ***environment);
void	free_content(t_post_quotes **content);
void	lexer_free(t_lexer **lexer);
void	ascii_sort(char **environment);
bool	pipe_export(t_lexer ***lexer);
void	lexer_free(t_lexer **lexer);

bool	check_export_for_quotes_check_start(
		t_lexer ***lexer, t_post_quotes	***content)
{
	if ((*lexer)[1] != NULL && (*lexer)[0]->args != NULL)
	{
		printf("Can't pipe export when setting a value\n");
		lexer_free((*lexer));
		free_content((*content));
		return (true);
	}
	if ((*lexer)[0]->args == NULL && (*lexer)[1] == NULL)
	{
		ascii_sort(*(get_env)());
		lexer_free((*lexer));
		free_content((*content));
		return (true);
	}
	if ((*lexer)[0]->args == NULL && (*lexer)[1] != NULL)
	{
		pipe_export(lexer);
		lexer_free((*lexer));
		free_content((*content));
		return (true);
	}
	return (false);
}

bool	mv_temp_to_real_export(int *i_export_content,
		char *cur_cont_noquotes, char ***export_content)
{
	int			j;
	char		**temp_var_from_no_quotes;

	temp_var_from_no_quotes = get_export_var(cur_cont_noquotes);
	if (check_if_array_str_is_empty(temp_var_from_no_quotes) == true)
		return (export_content_freeur(&temp_var_from_no_quotes), false);
	j = 0;
	if ((*i_export_content) % 2 == 1 && cur_cont_noquotes[0] == '=')
		free(temp_var_from_no_quotes[j++]);
	if ((*i_export_content) % 2 == 1 && cur_cont_noquotes[0] != '=')
		(*export_content)[(*i_export_content)++] = ft_strdup("");
	while (temp_var_from_no_quotes[j] != NULL)
	{
		if (temp_var_from_no_quotes[j + 1] == NULL
			&& get_last_char(cur_cont_noquotes) == '=')
		{
			free(temp_var_from_no_quotes[j]);
			break ;
		}
		(*export_content)[(*i_export_content)] = temp_var_from_no_quotes[j++];
		(*i_export_content)++;
	}
	return (free(temp_var_from_no_quotes), false);
}

void	go_through_content_get_export(int *i, t_post_quotes ***content, char ***export_content)
{
	int			i_export_content;

	i_export_content = 0;
	while ((*content)[(*i)] != NULL)
	{
		if ((*content)[(*i)]->is_quotes)
			(*export_content)[i_export_content++]
				= ft_strdup((*content)[(*i)]->content);
		else
		{
			if (is_all_space((*content)[(*i)]->content) == false)
			{
				mv_temp_to_real_export(&i_export_content,
					(*content)[(*i)]->content, export_content);
			}
			else if (i_export_content % 2 == 1)
				(*export_content)[i_export_content++] = ft_strdup("");
		}
		(*i)++;
	}
	if (i_export_content % 2 == 1)
		(*export_content)[i_export_content++] = ft_strdup("");
	(*export_content)[i_export_content] = NULL;
}

void	set_env_for_quotes(t_post_quotes ***content, t_lexer ***lexer, char ***export_content)
{
	int	i;

	i = 0;
	while ((*export_content)[i] != NULL)
	{
		if (ft_isdigit((*export_content)[i][0]) == 1)
		{
			printf("Minishell: export: '%s", (*export_content)[i]);
			if ((*export_content)[i + 1][0] != '\0')
				printf("=%s", (*export_content)[i + 1]);
			printf("': not a valid identifier\n");
			while ((*export_content)[i] != NULL)
				free((*export_content)[i++]);
			free((*export_content));
			lexer_free((*lexer));
			free_content(*content);
			return ;
		}
		set_env((*export_content)[i], (*export_content)[i + 1], get_env());
		i += 2;
	}
	free((*export_content));
	free_content(*content);
	lexer_free((*lexer));
}

bool	check_export_for_quotes(
		t_post_quotes ***content, t_lexer ***lexer)
{
	int			i;
	char		**export_content;
	char		*temp;

	if (is_str_export((*lexer)[0]->cmd) == false)
		return (false);
	if (check_export_for_quotes_check_start(lexer, content))
		return (true);
	export_content = malloc(sizeof(char *)
			* ((counte_num_new_var((*content)) * 2) + 1));
	if (is_str_export((*content)[0]->content))
		i = 1;
	else
	{
		temp = ft_strdup(&((*content)[0]->content[7]));
		free((*content)[0]->content);
		(*content)[0]->content = temp;
		i = 0;
	}
	go_through_content_get_export(&i, content, &export_content);

	set_env_for_quotes(content, lexer, &export_content);
	return (true);
}

int	count_char_unti_space(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (ft_isspace(str[i]))
			return (i);
		i++;
	}
	return (i);
}

int count_len_split_world(char *str)
{
	int		i;
	int		word_count;

	i = 0;
	word_count = 0;
	while (str[i] != '\0')
	{
		while (ft_isspace((str[i])) && str[i])
			i++;
		if (!(ft_isspace(str[i])) && str[i])
		{
			word_count++;
			i++;
		}
	}
	return (word_count);
}

int	count_len_split_world(char *str);
int	count_char_unti_space(char *str);

char	**initialize_str_split(char *str)
{
	char	**str_split;

	str_split = (char **)
		malloc(sizeof(char *)
			* (count_len_split_world(str) + 1));
	if (!str_split)
		exit(-1);
	return (str_split);
}

void	finish_writing(int *j, int *word_count, char **str_split, bool *writing)
{
	str_split[(*word_count)++][(*j)] = '\0';
	(*j) = 0;
	*writing = false;
}

void	split_logic(char *str, char **str_split, int *word_count, bool *writing)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i] != '\0')
	{
		if (ft_isspace(str[i]))
		{
			if (*writing)
				finish_writing(&j, word_count, str_split, writing);
			if (str[i + 1] != '\0' && !ft_isspace(str[i + 1]))
				str_split[*word_count] = (char *)malloc
					(sizeof(char) * (count_char_unti_space(&str[i + 1]) + 1));
		}
		else
		{
			if (i == 0)
				str_split[*word_count] = (char *)
					malloc(sizeof(char) * (count_char_unti_space(&str[i]) + 1));
			*writing = true;
			str_split[*word_count][j++] = str[i];
		}
		i++;
	}
}

char *remove_front_spaces(char *str);

char	**finalize_str_split(char **str, char **str_split, int word_count)
{
	int	j;

	j = 0;
	(*str) = remove_front_spaces((*str));
	while ((*str)[j] && !ft_isspace((*str)[j]))
		j++;
	if (!ft_isspace((*str)[j - 1]))
		str_split[word_count][j] = '\0';
	str_split[++word_count] = NULL;
	return (str_split);
}

char	**ft_split_world_at_all_spaces(char *str)
{
	int		word_count;
	bool	writing;
	char	**str_split;
	char	**str_return;

	str = ft_strdup(str);
	word_count = 0;
	writing = false;
	str_split = initialize_str_split(str);
	split_logic(str, str_split, &word_count, &writing);
	str_return = finalize_str_split(&str, str_split, word_count);
	if (str != NULL)
		free(str);
	return (str_return);
}

char	**remalloc_and_dup(char **all_var_rm, int all_var_rm_total)
{
	char	**all_var_rm_new;
	int		i;

	all_var_rm_new = malloc(sizeof(char *) * (all_var_rm_total));
	i = 0;
	while (all_var_rm[i] != NULL)
	{
		all_var_rm_new[i] = all_var_rm[i];
		i++;
	}
	free(all_var_rm);
	return (all_var_rm_new);
}

int		unset_env(char *name, char ***environment);

bool	check_unset_noquotes(t_lexer ***lexer)
{
	char	**var_prept;
	int		i;

	i = 0;
	if (is_str_unset((*lexer)[0]->cmd))
	{
		if ((*lexer)[1] != NULL)
		{
			printf("Can't pipe unset\n");
			return (lexer_free((*lexer)), true);
		}
		if ((*lexer)[0]->args == NULL)
			return (lexer_free((*lexer)), true);
		var_prept = ft_split_world_at_all_spaces((*lexer)[0]->args);
		i = 0;
		while (var_prept[i] != NULL)
			unset_env(var_prept[i++], get_env());
		i = 0;
		while (var_prept[i] != NULL)
		{
			free(var_prept[i++]);
		}
		return (free(var_prept), lexer_free((*lexer)), true);
	}
	return (false);
}

bool	check_unset_for_quotes(t_post_quotes	***content, t_lexer ***lexer)
{
	char	**all_var_rm;
	int		all_var_rm_num;
	int		all_var_rm_total;
	int		i;
	
	if (is_str_unset((*lexer)[0]->cmd) == false)
		return (false);
	if ((*lexer)[1] != NULL)
	{
		printf("Can't pipe unset\n");
		lexer_free((*lexer));
		free_content((*content));
		return (true);
	}
	if ((*lexer)[0]->args == NULL)
	{
		lexer_free((*lexer));
		free_content((*content));
		return (true);
	}
	if (is_str_unset((*content)[0]->content))
		i = 1;
	else
	{
		char *temp;
		temp = ft_strdup(&((*content)[0]->content[5]));
		free((*content)[0]->content);
		(*content)[0]->content = temp;
		i = 0;
	}
	while ((*content)[i] != NULL)
		i++;
	all_var_rm_total = i * 2 + 1;
	all_var_rm = malloc(sizeof(char *) * (all_var_rm_total));
	all_var_rm_num = 0;
	if (is_str_unset((*content)[0]->content))
		i = 1;
	else
		i = 0;
	while ((*content)[i])
	{
		if ((*content)[i]->is_quotes == true)
		{
			all_var_rm[all_var_rm_num] = ft_strdup((*content)[i]->content);
			i++;
		}
		else
		{
			char **all_var_rm_temp;
			int j;

			if (is_all_space((*content)[i]->content))
			{
				i++;
				continue ;
			}
			all_var_rm_temp = ft_split_world_at_all_spaces((*content)[i]->content);
			j = 0;
			while (all_var_rm_temp[j] != NULL)
				j++;
			if (j == 1)
			{
				all_var_rm[all_var_rm_num] = all_var_rm_temp[0];
				free(all_var_rm_temp);
				all_var_rm_num++;
				i++;
				continue ;
			}
			all_var_rm[all_var_rm_num] = NULL;
			all_var_rm_total = all_var_rm_total + j;
			all_var_rm = remalloc_and_dup(all_var_rm, all_var_rm_total);
			j = 0;
			while (all_var_rm_temp[j] != NULL)
			{
				all_var_rm[all_var_rm_num] = all_var_rm_temp[j];
				all_var_rm_num++;
				j++;
			}
			free(all_var_rm_temp);
			i++;
			continue ;
		}
		all_var_rm_num++;
	}
	all_var_rm[all_var_rm_num] = NULL;
	i = 0;
	while (all_var_rm[i] != NULL)
	{
		unset_env(all_var_rm[i], get_env());
		// free(all_var_rm[i]);
		i++;
	}
	i = 0;
	while (all_var_rm[i] != NULL)
	{
		free(all_var_rm[i]);
		i++;
	}
	free(all_var_rm);
	free_content(*content);
	lexer_free((*lexer));
	return (true);
}

