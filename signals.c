/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omathot <omathot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 11:31:43 by omathot           #+#    #+#             */
/*   Updated: 2023/06/17 14:45:05 by omathot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
need exectuable to run to have a behaviour. When waiting for prompt, does nothing.
ctrl + \ = ^\Quit: 3
ctrl + C = stop executable
ctrl + D = does not stop exec and prints ^D

when waiting for prompt:
ctrl + D = exit
ctrl + C = \n

when somehting is typed into prompt:
c = new line;
\ && D = nothing

when terminal window is started and empty, D will exit terminal application.
ctrl d can also be used to logout somehow just keep that in mind.
*/

void	signal_catcher(int sig)
{
	if (sig == SIGINT)
	{
		// printf("I am not stoping f u\n");
		write(1, "\nminishell_OS_1.0$  ", 21);
		// readline("minishell_OS_1.0$ 😀 ");
		return ;
	}
}

void	slash_catch(int sig)
{
	if (sig == SIGQUIT)
	{
		printf("I am not stoping f me\n");
		// readline("minishell_OS_1.0$ 😀 ");
		return ;
	}
}

void	manage_signals(void)
{
	struct sigaction	s1;
	struct sigaction	s2;
	// struct sigaction	s3;

	s1.sa_handler = &signal_catcher;
	sigaction(SIGINT, &s1, NULL);
	s2.sa_handler = &slash_catch;
	sigaction(SIGQUIT, &s2, NULL);
}



/*
1. termine signals - ctrl + d - voir what imprimes 2 fois
2. 
*/