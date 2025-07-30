/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsahloul <lsahloul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:54:29 by lsahloul          #+#    #+#             */
/*   Updated: 2025/07/30 20:12:18 by lsahloul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    butter_free(t_shell *shell)
{
    if (shell->input)
	{
	free(shell->input);
	shell->input = NULL;
	}
}

int main(int argc, char **argv, char **envp)
{
    t_shell	shell;

    (void)argc;
    (void)argv;
    (void)envp;
	shell.input = NULL;
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
