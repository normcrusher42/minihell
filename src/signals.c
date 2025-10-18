/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsahloul <lsahloul@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 18:32:04 by lsahloul          #+#    #+#             */
/*   Updated: 2025/10/18 18:32:04 by lsahloul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#define RL_USE_OLD_TERMIOS
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include "minishell.h"

// Handles SIGINT (Ctrl+C) by printing a new prompt line.
void	handle_sigint(int sig)
{
	(void)sig;
	rl_on_new_line();
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// Handles SIGQUIT (Ctrl+\) by erasing the ^\ from the terminal.
void	handle_sigquit(int sig)
{
	(void)sig;
}

// Disables echoing of control characters like ^C and ^\ in the terminal.
void	disable_echoctl(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return ;
	term.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
		return ;
}

// Initializes signal handlers for SIGINT and SIGQUIT.
void	init_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	disable_echoctl();
	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sa_quit.sa_handler = handle_sigquit;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sigaction(SIGQUIT, &sa_quit, NULL);
}
