/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 11:31:43 by omathot           #+#    #+#             */
/*   Updated: 2023/11/20 14:35:21 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
need exectuable to run to have a behaviour. 
When waiting for prompt, does nothing.
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

/*
this doesn't work on linux scohol terminal, evne thouhg it clearly should.
Works everywhere else, more portable and should be used instead.
	write(STDERR_FILENO, "\b\b  ", 4);
	write(STDERR_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
*/

void	signal_catcher(int sig)
{
	if (sig == SIGINT)
	{
		write(STDERR_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_exit_status = 130;
	}
	(void) sig;
}

void	slash_catch(int sig)
{
	if (sig == SIGQUIT)
	{
		rl_redisplay();
		g_exit_status = 131;
	}
}

void	manage_signals(void)
{
	struct sigaction	s1;
	struct sigaction	s2;

	s1.sa_handler = &signal_catcher;
	sigemptyset(&(s1.sa_mask));
	sigaddset(&s1.sa_mask, SIGINT);
	s1.sa_flags = 0;
	sigaction(SIGINT, &s1, NULL);
	s2.sa_handler = &slash_catch;
	sigemptyset(&(s2.sa_mask));
	sigaddset(&s2.sa_mask, SIGINT);
	s2.sa_flags = 0;
	sigaction(SIGQUIT, &s2, NULL);
	return ;
}
