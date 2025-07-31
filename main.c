/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:54:29 by lsahloul          #+#    #+#             */
/*   Updated: 2025/07/31 17:55:45 by nanasser         ###   ########.fr       */
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

void	free_env(char ***envp)
{
	int	i;

	i = 0;
	while (*envp[i])
	{
		free(*envp[i]);
		*envp[i++] = NULL;
	}
	free(*envp);
	*envp = NULL;
}

void	init_shell(char **envp, t_shell	*shell)
{
	
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;

	(void)ac;
	(void)av;
	shell = (t_shell){0};
	init_shell(envp, &shell);
	while (1)
	{
		shell.input = readline("minishell$ ");
		if (!shell.input)
			break ;
		if (*shell.input)
			add_history(shell.input);
		butter_free(&shell);
	}
	return (0);
}
