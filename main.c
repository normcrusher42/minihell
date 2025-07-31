/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:54:29 by lsahloul          #+#    #+#             */
/*   Updated: 2025/07/30 21:28:58 by nanasser         ###   ########.fr       */
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
	if (ac >= 2)
	{
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
	}
	return (0);
}
