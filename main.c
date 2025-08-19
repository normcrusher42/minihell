/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsahloul <lsahloul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 19:22:59 by lsahloul          #+#    #+#             */
/*   Updated: 2025/08/16 15:00:00 by lsahloul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"           /* ensure this includes parsing/cmd_table.h */

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

static void	init_shell(char **envp, t_shell *shell)
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
		if (*shell.input)
		{
			t_token	*tok;

			tok = tokenize(shell.input);
			if (tok)
			{
				/* 1) expand + strip quotes */
				process_all_tokens(tok, shell.envp, shell.last_exit_status);

				/* 2) NEW: build command table (Beans parser test) */
				{
					t_cmd	*cmds;
					int		ncmd;
					int		ok;

					cmds = NULL;
					ncmd = 0;
					ok = parse_command_table(tok, &cmds, &ncmd,
							&shell.last_exit_status);
					if (ok)
					{
#ifdef PARSE_DEBUG
						print_cmd_table(cmds, ncmd); /* test output */
#endif
						/* 3) (later) pass cmds to executor here */
						free_cmd_table(cmds, ncmd);
					}
					/* on error, parse_command_table already set status=258 */
				}
				free_tokens(tok);
			}
		}
		butter_free(&shell);
	}
	free_arr(&shell.envp, NO);
	return (0);
}
