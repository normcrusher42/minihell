/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsahloul <lsahloul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 19:22:59 by lsahloul          #+#    #+#             */
/*   Updated: 2025/08/16 20:30:00 by lsahloul         ###   ########.fr       */
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

/* return 1 if we should print the parsed table */
static int	should_debug_parse(void)
{
	const char	*val;

	val = getenv("MSH_PARSE_DEBUG");
	if (!val || !*val)
		return (0);
	return (1);
}

static void	process_line_tokens(t_shell *sh)
{
	t_token	*tok;
	t_cmd	*cmds;
	int		ncmd;
	int		ok;

	tok = tokenize(sh->input);
	if (!tok)
		return ;
	process_all_tokens(tok, sh->envp, sh->last_exit_status);
	cmds = NULL;
	ncmd = 0;
	ok = parse_command_table(tok, &cmds, &ncmd, &sh->last_exit_status);
	if (ok)
	{
		if (should_debug_parse())
			print_cmd_table(cmds, ncmd);
		/* later: execute_job(cmds, ncmd, sh); */
		free_cmd_table(cmds, ncmd);
	}
	free_tokens(tok);
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
			process_line_tokens(&shell);
		butter_free(&shell);
	}
	free_arr(&shell.envp, NO);
	return (0);
}
