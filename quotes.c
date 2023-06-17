/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sboulain <sboulain@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 11:53:41 by omathot           #+#    #+#             */
/*   Updated: 2023/06/17 17:18:06 by sboulain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Handle ’ (single quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence.
Handle " (double quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence except for $ (dollar sign).
*/

int		*get_doll_position(char *str);
char	*handle_expand_doll(char *str);

// here *str is &str[3] from main.
void	check_quotes(char *str)
{
	// int	i;

	// i = 0;
	// while (str[i])
	// {
		// 39 is ascii value for single quotes BUT, \ is in front of it
		if (str[0] == '\'' && str[ft_strlen(str) - 1] == '\'')
			printf("%s\n", str);
		// 34 is ascii value for double quotes,
		// if (str[0] == '\"' && str[ft_strlen(str) - 1] == '\"')
		else
		{
			char *str_temp;
			str_temp = handle_expand_doll(str);
			printf("%s\n", str_temp);
			free(str_temp);
			system("leaks -q minishell");
		}
	// }
}

char	**spit_text_args(char *str, int	*doll_pos);
char	*ft_strjoin_double_str(char **spit_text);
void	split_test_freeur(char **spit_text);

char	*handle_expand_doll(char *str)
{
	int	*doll_pos;
	char **spit_text;
	char *final_str;

	doll_pos = get_doll_position(str);
	if (doll_pos[0] == -1)
		{
			free(doll_pos);
			return (ft_strdup(str));
		}
	spit_text = spit_text_args(str, doll_pos);
	final_str = ft_strjoin_double_str(spit_text);
	split_test_freeur(spit_text);
	free(doll_pos);
	return (final_str);
}

char	*ft_strjoin_double_str(char **spit_text)
{
	int	i;
	int	j;
	int	size_str;
	char *str;

	size_str = 0;
	i = 0;
	while (spit_text[i] != NULL)
	{
		size_str = size_str + ft_strlen(spit_text[i]);
		i++;
	}
	str = malloc(sizeof(char) * (size_str + 1));
	if (!str)
		exit(EXIT_FAILURE);
	size_str = 0;
	i = 0;
	while (spit_text[i] != NULL)
	{
		j = 0;
		while (spit_text[i][j] != '\0')
		{
			str[size_str] = spit_text[i][j];
			size_str++;
			j++;
		}
		i++;
	}
	str[size_str] = '\0';
	return (str);
}

void	split_test_freeur(char **spit_text)
{
	int	i;

	i = 0;
	while (spit_text[i] != NULL)
	{
		if (i % 2 == 0)
			free(spit_text[i]);
		i++;
	}
	free(spit_text);
	spit_text = NULL;
	
}

char *ft_strdup_intil_index_n(char *s1, int n)
{
	int		i;
	char	*c;

	i = 0;
	c = malloc(sizeof(char) * (n + 2));
	if (!c)
		return (NULL);
	while (i <= n)
	{
		c[i] = s1[i];
		i++;
	}
	c[i] = '\0';
	return (c);
}

bool	ft_isspace(unsigned char c);

int	find_next_space(char *str)
{
	int	i;

	i = 0;
	while (!ft_isspace(str[i]))
	{
		if (str[i] == '\0')
			return (-1);
		i++;
	}
	return (i);
}

int	find_next_quote(char *str)
{
	int	i;

	i = 0;
	while (!(str[i] == '\"'))
	{
		if (str[i] == '\0')
			return (-1);
		i++;
	}
	return (i);
}


char	**spit_text_args(char *str, int	*doll_pos)
{
	int		i;
	char	**string_split;
	int		num_doll;
	char	*temp_str;
	int		total_parts;

	num_doll = 0;
	while (doll_pos[num_doll] != -1)
		num_doll++;
	total_parts = (num_doll * 2) + 1;
	string_split = malloc(sizeof(char *) * (total_parts + 2));
	i = 0;
	num_doll = 0;
	while (i < (total_parts))
	{
		if (i % 2 == 0)
		{
			if (i == 0)
				string_split[i] = ft_strdup_intil_index_n(str, doll_pos[num_doll] - 1);
			else
			{
				if (doll_pos[num_doll] != -1)
					if (find_next_space(&str[doll_pos[num_doll - 1]] + 1) == -1 || find_next_space(&str[doll_pos[num_doll - 1]] + 1) > doll_pos[num_doll] - (doll_pos[num_doll - 1] + 1))
						string_split[i] = ft_strdup("\0");
					else
						string_split[i] = ft_strdup_intil_index_n(&str
							[doll_pos[num_doll - 1] + find_next_space(&str[doll_pos[num_doll - 1]])],
							doll_pos[num_doll]
							- (doll_pos[num_doll - 1] + find_next_space(&str[doll_pos[num_doll - 1]])) - 1);
				else
					if (find_next_space(&str[doll_pos[num_doll - 1]]) == -1)
						if (find_next_quote(&str[doll_pos[num_doll - 1]]) == -1)
							string_split[i] = ft_strdup("\0");
						else
							string_split[i] = ft_strdup("\"");
					else
						string_split[i] = ft_strdup_intil_index_n(&str
							[doll_pos[num_doll - 1] + find_next_space(&str[doll_pos[num_doll - 1]])],
							ft_strlen(&str
							[doll_pos[num_doll - 1] + find_next_space(&str[doll_pos[num_doll - 1]])]));
			}
		}
		else
		{
			if (find_next_space(&str[doll_pos[num_doll]]) == -1)
			{
				if (doll_pos[num_doll + 1] != -1)
					temp_str = ft_strdup_intil_index_n(&str[doll_pos[num_doll] + 1], doll_pos[num_doll + 1] - (doll_pos[num_doll] + 2));
				else
				{
					if (find_next_quote(&str[doll_pos[num_doll] + 1]) - 1 != -2)
						temp_str = ft_strdup_intil_index_n(&str[doll_pos[num_doll] + 1], find_next_quote(&str[doll_pos[num_doll] + 1]) - 1);
					else
					{
						printf("%d\n", doll_pos[num_doll] + 1);
						temp_str = ft_strdup(&str[doll_pos[num_doll] + 1]);
					}
				}
			}
			else if (doll_pos[num_doll + 1] != -1 && find_next_space(&str[doll_pos[num_doll]] + 1) > doll_pos[num_doll + 1] - (doll_pos[num_doll] + 1))
			{
				printf("\n %d, %d\n", doll_pos[num_doll + 1],  doll_pos[num_doll]);
				temp_str = ft_strdup_intil_index_n(&str[doll_pos[num_doll] + 1], doll_pos[num_doll + 1] - (doll_pos[num_doll] + 2));
			}
			else
				temp_str = ft_strdup_intil_index_n(&str[doll_pos[num_doll] + 1], find_next_space(&str[doll_pos[num_doll]] + 1) - 1);
			printf("\n%s\n", temp_str);
			string_split[i] = getenv(temp_str);
			// need to manage if faled
			free(temp_str);
			num_doll++;
		}
		i++;
	}
	string_split[i] = NULL;
	return (string_split);
}

int	*get_doll_position(char *str)
{
	int	i;
	int	num_doll;
	int	*doll_pos;

	i = 0;
	num_doll = 0;
	while (str[i])
	{
		if (str[i] == '$')
			num_doll++;
		i++;
	}
	doll_pos = malloc(sizeof(int) * (num_doll + 1));
	if (!doll_pos)
		exit(EXIT_FAILURE);
	i = 0;
	num_doll = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '$')
		{
			doll_pos[num_doll] = i;
			num_doll++;
		}
		i++;
	}
	doll_pos[num_doll] = -1;
	return (doll_pos);
}

// char **all;
// malloc (sizeof(char *) * parts);
// all[0] = strconais;
// all[1] = getenv(str);

// char *finish;
// finish(len2char())

// 1. look at first open - pattern matching     'asudgadagg"ausydgasdas"adhad';
// 2. getenv 