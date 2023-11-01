/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 13:29:45 by omathot           #+#    #+#             */
/*   Updated: 2023/10/31 22:19:42 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		main(void);
char	*read_user_input(bool quotes_test);
char	is_prompt_empty(char *str);
int executer(t_lexer **lexer, t_pipedata *data);
t_lexer	**main_parser(char *str);
void	check_quotes(char **str_og, t_post_quotes ***content);
t_lexer	**parser_with_quotes(t_post_quotes **content);
bool	check_export_for_quotes(t_post_quotes	***content, t_lexer ***lexer);

bool is_str_export(char *str);
char **get_export_var(char *arg_of_export);
bool	check_unset_for_quotes(t_post_quotes	***content, t_lexer ***lexer);
void	free_double_array(char **list_of_tokenid);

void	lexer_free(t_lexer **lexer)
{
	int i;
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

void free_content(t_post_quotes **content)
{
	int i;

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
void export_content_freeur(char ***export_content);
void	ascii_sort(char **environment);
bool	check_unset_noquotes(t_lexer ***lexer);

bool export_andle_no_quotes(t_lexer ***lexer)
{
	char **var_prept;
	int i;

	if (is_str_export((*lexer)[0]->cmd))
	{
		if ((*lexer)[1] != NULL)
		{
			printf("Can't pipe export\n");
			lexer_free((*lexer));
			return (true);
		}
		if ((*lexer)[0]->args == NULL)
		{
			ascii_sort(*(get_env)());
			lexer_free((*lexer));
			return (true);
		}
		var_prept = get_export_var((*lexer)[0]->args);
		i = 0;
		while (var_prept[i] != NULL)
		{
			if (i % 2 == 0)
				printf("(%s)", var_prept[i]);
			else 
				printf(" = (%s)\n", var_prept[i]);
			i++;
		}
	printf("\n");
		i = 0;
		while (var_prept[i] != NULL)
		{
			if (ft_isdigit(var_prept[i][0]) == 1)
			{
				printf("Minishell: export: `%s", var_prept[i]);
				if (var_prept[i + 1][0] != '\0')
					printf("=%s", var_prept[i + 1]);
				printf("': not a valid identifier\n");
				while (var_prept[i] != NULL)
				{
					free(var_prept[i]);
					i++;
				}
				free(var_prept);
				lexer_free((*lexer));
				return (true);
			}
			set_env(var_prept[i], var_prept[i + 1], get_env());
			i += 2;
		}
	}
	else
		return (false);
	lexer_free((*lexer));
	free(var_prept);
	return (true);
}

int    main(void)
{
    char			*str;
    t_lexer			**lexer;
    bool			quotes_test;
	t_post_quotes	**content;
	// int	i;e

	manage_signals();
	quotes_test = true;
	while (1)
	{
		// intial prompt print
		// read user input
		content = NULL;

		str = ft_strdup(read_user_input(quotes_test));
		if (is_prompt_empty(str) == 0)
			continue ;
		// if (str == NULL)
		// 	continue ;
		if (quotes_test)
		{
			check_quotes(&str, &content);
		}
		// parse user input
		// if (!quotes_test)
		// {
			// pause();
			// puts("");
			// puts(str);

			if (content == NULL)
				lexer = main_parser(str);
			else
			{
				lexer = parser_with_quotes(content);
				if (check_export_for_quotes(&content, &lexer))
					continue ;
				if (check_unset_for_quotes(&content, &lexer))
					continue ;
				// int i;
				if (is_str_export(lexer[0]->cmd))
				{
					if (lexer[1] != NULL)
						printf("Can't pipe export\n");
					else
						get_export_var(lexer[0]->args);
					lexer_free(lexer);
					free_content(content);
					continue ;
				}
				free_content(content);
			}
			free(str);
			// i = 0;
			// c'est pas idea mais c'est un depart


			int i;
			i = 0;
			while (lexer[i] != NULL)
			{
				// printf("arg was before: %s\n", lexer[i]->args);
				// lexer[i]->args = replace_doll_question_to_number_with_free(lexer[i]->args, 69);
				printf("\nlexer[%d]\n cmd: (%s)\n", i, lexer[i]->cmd);
				printf("args: (%s)\n", lexer[i]->args);
				printf("tokenid: (%s)\n", lexer[i]->tokenid);
				printf("file: (%s)\n", lexer[i]->file);
				printf("flags: (%s)\n", lexer[i]->flags);
				i++;
			}
			if (export_andle_no_quotes(&lexer))
				continue ;
			if (check_unset_noquotes(&lexer))
				continue ;
			else
				piping(lexer);
			// pause();
			// optiona: wait for return value.
		// }
		lexer_free(lexer);
			// break ;
	}
	return (0);
}

char	*read_user_input(bool quotes_test)
{
	char	*str;

	str = readline("😎 minishell_OS_1.0$ ");
	if (!quotes_test)
		add_history(str);
	// if (EOF)
	// {
	// 	write(1, "exit\n", 5);
	// 	exit(EXIT_SUCCESS);
	// }
	return (str);
}


char	is_prompt_empty(char *str)
{
	int		i;
	char	is_valid;

	i = 0;
	is_valid = 0;
	while (str[i])
		i++;
	if (i > 0)
		is_valid = 1;
	return (is_valid);
}