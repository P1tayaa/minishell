/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 22:57:07 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 22:57:14 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**split_spaces(char *str)
{
	int		j;
	int		word_count;
	char	**str_split;

	str_split = (char **)malloc(sizeof(char *)
			* (count_len_split_world(str) + 1));
	if (!(str_split))
		exit(-1);
	word_count = 0;
	j = 0;
	j = finish_or_malloc_word(str, &str_split, &word_count);
	if (ft_isspace(str[ft_strlen(str) - 1]) == 0)
	{
		str_split[word_count][j] = '\0';
		word_count++;
	}
	str_split[word_count] = NULL;
	return (str_split);
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
		var_prept = split_spaces((*lexer)[0]->args);
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

void	checks_andset_else(t_post_quotes ***content, int *i)
{
	char	*temp;

	temp = ft_strdup(&((*content)[0]->content[5]));
	free((*content)[0]->content);
	(*content)[0]->content = temp;
	(*i) = 0;
}

int	checks_andset_i(t_lexer ***lexer, t_post_quotes ***content, int *i)
{
	if ((*lexer)[1] != NULL)
	{
		printf("Can't pipe unset\n");
		lexer_free((*lexer));
		free_content((*content));
		return (1);
	}
	if ((*lexer)[0]->args == NULL)
	{
		lexer_free((*lexer));
		free_content((*content));
		return (1);
	}
	if (is_str_unset((*content)[0]->content))
		(*i) = 1;
	else
		checks_andset_else(content, i);
	while ((*content)[(*i)] != NULL)
		(*i)++;
	return (0);
}
