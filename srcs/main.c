/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 13:29:45 by omathot           #+#    #+#             */
/*   Updated: 2023/11/25 22:54:56 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	g_exit_status;

void	lexer_free(t_lexer **lexer)
{
	int	i;

	i = 0;
	while (lexer[i] != NULL)
	{
		if (lexer[i]->cmd != NULL)
			free(lexer[i]->cmd);
		if (lexer[i]->args != NULL)
			free(lexer[i]->args);
		if (lexer[i]->file != NULL)
			free(lexer[i]->file);
		if (lexer[i]->flags != NULL)
			free(lexer[i]->flags);
		lexer[i]->cmd = NULL;
		lexer[i]->args = NULL;
		lexer[i]->file = NULL;
		lexer[i]->flags = NULL;
		free(lexer[i]);
		i++;
	}
	free(lexer);
}

void	free_content(t_post_quotes **content)
{
	int	i;

	i = 0;
	while (content[i] != NULL)
	{
		free(content[i]->content);
		free(content[i]);
		i++;
	}
	free(content);
}

void	set_env(char *name, char *value, char ***environment);
char	***get_env(void);
void	print_export(char ***environment);
void	export_content_freeur(char ***export_content);
void	ascii_sort(char **environment);
bool	check_unset_noquotes(t_lexer ***lexer);

void	handle_export_digit_err(t_lexer ***lexer, char ***var_prept, int *i)
{
	while ((*var_prept)[(*i)] != NULL)
	{
		if (ft_isdigit((*var_prept)[(*i)][0]) == 1)
		{
			printf("Minishell: export: `%s", (*var_prept)[(*i)]);
			if ((*var_prept)[(*i) + 1][0] != '\0')
				printf("=%s", (*var_prept)[(*i) + 1]);
			printf("': not a valid identifier\n");
			while ((*var_prept)[(*i)] != NULL)
			{
				free((*var_prept)[(*i)]);
				(*i)++;
			}
			free((*var_prept));
			lexer_free((*lexer));
			break ;
		}
		set_env((*var_prept)[(*i)], (*var_prept)[(*i) + 1], get_env());
		(*i) += 2;
	}
}

int	initialize_and_checks(t_post_quotes ***content,
		bool quotes_test, char **str)
{
	manage_signals(0);
	(*content) = NULL;
	(*str) = read_user_input(quotes_test);
	if ((*str) == NULL)
		return (1);
	if (quotes_test)
		check_quotes_diff(str, content);
	return (0);
}

int	main(void)
{
	char			*str;
	t_lexer			**lexer;
	bool			quotes_test;
	t_post_quotes	**content;

	quotes_test = true;
	while (1)
	{
		if (initialize_and_checks(&content, quotes_test, &str) == 1)
			break ;
		if (content == NULL)
			lexer = main_parser(str);
		else
		{
			if (lexer_with_quotes(&lexer, &content, &str) == 1)
				continue ;
		}
		free(str);
		if (noquotes_into_pipes(&lexer) == 1)
			continue ;
		finish_output(lexer);
		lexer_free(lexer);
	}
	return (0);
}
