/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsahloul <lsahloul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:54:29 by lsahloul          #+#    #+#             */
/*   Updated: 2025/07/31 18:47:25 by lsahloul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	butter_free(t_shell *shell)
{
	if (shell->input)
	{
		free(shell->input);
		shell->input = NULL;
	}
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;

	(void)ac;
	(void)av;
	(void)envp;
	shell = (t_shell){0};
	init_signals();
	while (1)
	{
		shell.input = readline("minishell$ ");
		if (!shell.input)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (*shell.input)
			add_history(shell.input);
		butter_free(&shell);
	}
	return (0);
}
