/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: team                                           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:54:29 by team              #+#    #+#             */
/*   Updated: 2025/08/10 21:00:00 by team             ###   ########.fr       */
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

static char	**dup_env(char **envp)
{
	char	**new_envp;
	int		count;

	count = ft_arrlen(envp);
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
	init_signals();
	while (1)
	{
		shell.input = readline("miniOdy$ ");
		if (!shell.input)
			break ;
		if (*shell.input)
			add_history(shell.input);

		/* Week 2 flow (stub):
		 * 1) tokenize -> expand -> remove quotes
		 * 2) (later) parse into command table
		 * 3) (later) exec
		 */
		/* Example: just tokenize & expand for now */
		if (*shell.input)
		{
			t_token *tok = tokenize(shell.input);
			if (tok)
			{
				process_all_tokens(tok, shell.envp, shell.last_exit_status);
				/* TODO: parse & exec next weeks */
				free_tokens(tok);
			}
		}
		butter_free(&shell);
	}
	free_arr(&shell.envp, NO);
	return (0);
}
