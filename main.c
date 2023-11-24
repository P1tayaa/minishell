/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 13:29:45 by omathot           #+#    #+#             */
/*   Updated: 2023/11/23 17:37:11 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		main(void);
char	*read_user_input(bool quotes_test);
char	is_prompt_empty(char *str);
int		executer(t_lexer **lexer, t_pipedata *data);
t_lexer	**main_parser(char *str);
void	check_quotes(char **str_og, t_post_quotes ***content);
t_lexer	**parser_with_quotes(t_post_quotes **content);
bool	check_export_for_quotes(t_post_quotes	***content, t_lexer ***lexer);
bool	is_str_export(char *str);
char	**get_export_var(char *arg_of_export);
bool	check_unset_for_quotes(t_post_quotes	***content, t_lexer ***lexer);
void	free_double_array(char **list_of_tokenid);
void	pipe_export(t_lexer ***lexer);

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

void	handle_export_var_noquotes(t_lexer ***lexer)
{
	char	**var_prept;
	int		i;

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
	handle_export_digit_err(lexer, &var_prept, &i);
	lexer_free((*lexer));
	free(var_prept);
}

bool	export_andle_no_quotes(t_lexer ***lexer)
{
	if (is_str_export((*lexer)[0]->cmd) == false)
		return (false);
	if ((*lexer)[1] != NULL && (*lexer)[0]->args != NULL)
	{
		printf("Can't pipe export when setting a value\n");
		lexer_free((*lexer));
		return (true);
	}
	if ((*lexer)[0]->args == NULL && (*lexer)[1] == NULL)
	{
		ascii_sort(*(get_env)());
		lexer_free((*lexer));
		return (true);
	}
	if ((*lexer)[0]->args == NULL && (*lexer)[1] != NULL)
	{
		pipe_export(lexer);
		lexer_free((*lexer));
		return (true);
	}
	handle_export_var_noquotes(lexer);
	return (true);
}

void	restore_default_sigint_handling(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
}

void	finish_output(t_lexer **lexer)
{
	if (ft_strlen(lexer[0]->cmd) == 3)
		if ((ft_memcmp(lexer[0]->cmd, "cat", 3) == 0)
			&& lexer[1] == NULL && lexer[0]->args == NULL)
			write(2, "\033[18D\033[K", 8);
}

int	noquotes_into_pipes(t_lexer ***lexer)
{
	if (export_andle_no_quotes(lexer))
		return (1);
	if (check_unset_noquotes(lexer))
		return (1);
	else
		piping((*lexer));
	return (0);
}

int	lexer_with_quotes(t_lexer ***lexer, t_post_quotes ***content, char **str)
{
	(*lexer) = parser_with_quotes((*content));
	if (check_export_for_quotes(content, lexer)
		|| check_unset_for_quotes(content, lexer))
	{
		if ((*str))
		{
			free((*str));
			(*str) = NULL;
		}
		return (1);
	}
	free_content((*content));
	return (0);
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
		check_quotes(str, content);
	return (0);
}

int	main(void)
{
	char			*str;
	t_lexer			**lexer;
	bool			quotes_test;
	t_post_quotes	**content;
	// int				i;

	// i = 0;
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
		// while (lexer[i] != NULL)
		// {
		// 	printf("cmd = (%s)\n", lexer[i]->cmd);
		// 	printf("args = (%s)\n", lexer[i]->args);
		// 	printf("flags = (%s)\n", lexer[i]->flags);
		// 	printf("tokenid = (%s)\n", lexer[i]->tokenid);
		// 	printf("file = (%s)\n", lexer[i]->file);
		// 	printf("i = (%i)\n\n", i);
		// 	i++;
		// }
		free(str);
		if (noquotes_into_pipes(&lexer) == 1)
			continue ;
		finish_output(lexer);
		lexer_free(lexer);
	}
	return (0);
}

char	*read_user_input(bool quotes_test)
{
	char	*str;
	char	*str_return;

	rl_catch_signals = 0;
	while (1)
	{
		str = readline("minishell_OS_1.0$ ");
		if (str == NULL)
		{
			printf("\n");
			break ;
		}
		if (!quotes_test && str && *str)
			add_history(str);
		break ;
	}
	if (str == NULL)
		return (NULL);
	str_return = ft_strdup(str);
	if (str)
		free(str);
	return (str_return);
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
