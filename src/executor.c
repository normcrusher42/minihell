/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 15:56:47 by nanasser          #+#    #+#             */
/*   Updated: 2025/10/10 15:56:47 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	setup_heredocs(t_shell *sh)
{
	int	i;

	i = -1;
	while (++i < sh->cmds->redir_count)
		if (sh->cmds->redirs[i].type == R_HEREDOC)
			if (handle_heredoc(&sh->cmds->redirs[i], sh) == -1)
				return (130);
	return (0);
}

// Runs a single cmd passed and checks if its a builtin or a program
int	execute_command(char ***env, t_shell *sh)
{
	pid_t	pid;
	int		heredoc_status;

	if ((!sh->cmds->av || !sh->cmds->av[0]) && sh->cmds->redir_count == 0)
		return (0);
	if (sh->cmds->av && is_builtin(sh->cmds->av[0]))
		return (init_and_exec_builtins(sh->cmds->av, env, sh));
	heredoc_status = setup_heredocs(sh);
	if (heredoc_status != 0)
		return (heredoc_status);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == 0)
		handle_execution(sh, env);
	else if (pid > 0)
		return (handle_parent_process(pid, sh));
	perror(RED "Well well, how did we get here? That's embarassing." RESET);
	call_janitor(sh);
	exit(2);
}
