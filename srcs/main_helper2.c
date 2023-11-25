/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 22:47:34 by oscarmathot       #+#    #+#             */
/*   Updated: 2023/11/25 22:52:51 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**get_export_var(char *arg_of_export);
void	handle_export_digit_err(t_lexer ***lexer, char ***var_prept, int *i);
void	lexer_free(t_lexer **lexer);

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
