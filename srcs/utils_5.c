/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 22:58:05 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 22:58:09 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	go_through_content_get_export(int *i,
		t_post_quotes ***content, char ***export_content)
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

void	set_env_for_quotes(t_post_quotes ***content,
		t_lexer ***lexer, char ***export_content)
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
