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

// Increases or initializes the SHLVL environment variable.
void	update_shlvl(char ***envp)
{
	char	*val;
	int		lvl;
	char	*new_val;

	val = get_env_value(*envp, "SHLVL");
	if (!val)
	{
		set_env_value(envp, "SHLVL", "1", 0);
		return ;
	}
	lvl = ft_atoi(val);
	if (lvl < 0 || lvl > 1000)
		lvl = 1;
	else
		lvl++;
	new_val = ft_itoa(lvl);
	if (!new_val)
		return ;
	set_env_value(envp, "SHLVL", new_val, 0);
	free(new_val);
}

// Prepares shell environment variables by copying them (otherwise, create one).
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
	update_shlvl(&shell->envp);
}

// Return 1 if we should print the parsed table
static int	should_debug_parse(void)
{
	const char	*val;

	val = getenv("MSH_PARSE_DEBUG");
	if (!val || !*val)
		return (0);
	return (1);
}

// fine. I'll do it myself.
// Main parsing work that tokenizes and processes the input before executing
static void	process_line_tokens(t_shell *sh)
{
	int		ok;

	tokenize(sh->input, sh);
	process_all_tokens(sh, sh->envp);
	ok = parse_command_table(sh, &g_last_status);
	if (ok)
	{
		if (should_debug_parse())
			print_cmd_table(sh);
		g_last_status = execute_job(sh);
		free_cmd_table(sh);
	}
	free_tokens(sh);
}

/*   minishell brought to you by none other than @Nasser and @Leen!!!      */
/*     Expect 80% of what you see to give you "fever dream" vibes          */
/*                                        -if you know, you know-          */

// le shelling Magie 𝓬𝓸𝓶𝓶𝓮𝓷𝓬𝓮
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
		butter_free_input(&shell);
	}
	clear_history();
	free_arr(&shell.envp, NO);
	ft_printf("exit\n");
	return (g_last_status);
}
