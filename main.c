/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:54:29 by lsahloul          #+#    #+#             */
/*   Updated: 2025/07/31 19:05:15 by nanasser         ###   ########.fr       */
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
	while ((*envp)[i])
	{
		free((*envp)[i]);
		(*envp)[i++] = NULL;
	}
	free(*envp);
	*envp = NULL;
}

char	**dup_env(char **envp)
{
	char	**new_envp;
	int		count;

	count = 0;
	while (envp[count])
		count++;
	new_envp = malloc(sizeof(char *) * (count + 1));
	if (!new_envp)
		return (NULL);
	count = -1;
	while (envp[++count])
		new_envp[count] = ft_strdup(envp[count]);
	new_envp[count] = NULL;
	return (new_envp);
}

static void	init_shell(char **envp, t_shell	*shell)
{
	char	cwd[1024];

	if (envp && envp[0])
		shell->envp = dup_env(envp);
	else
	{
		shell->envp = malloc(sizeof(char *) * 2);
        if (!shell->envp)
            return ;
		if (getcwd(cwd, sizeof(cwd)))
		{
			shell->envp[0] = ft_strjoin("PWD=", cwd);
			shell->envp[1] = NULL;
		}
		else
			shell->envp[0] = NULL;
	}

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
	if (shell.envp)
		free_env(&shell.envp);
	return (0);
}
