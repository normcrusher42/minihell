/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   driver.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 15:56:28 by nanasser          #+#    #+#             */
/*   Updated: 2025/10/10 15:56:28 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_pipeline(t_pipeinfo *p, t_cmd *cmds, t_shell *sh)
{
	pid_t	pid;

	if (p->i < p->n - 1 && pipe(p->pipefd) == -1)
		return (perror("pipe"), sh->ex_st = 1);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), sh->ex_st = 1);
	if (pid == 0)
		run_child(&cmds[p->i], sh, p);
	handle_parent(p);
	return (0);
}

// Manages the entire pipeline execution with forking and piping.
int	run_pipeline(t_cmd *cmds, int n, t_shell *sh)
{
	t_pipeinfo	p;
	int			heredoc_status;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	heredoc_status = setup_heredocs(cmds, n, sh);
	if (heredoc_status != 0)
		return (heredoc_status);
	p.prev_fd = -1;
	p.i = -1;
	p.n = n;
	while (++p.i < n)
		if (execute_pipeline(&p, cmds, sh))
			return (sh->ex_st);
	return (handle_pipeline_status(sh));
}

// Executes the entire job, either a single command or a pipeline.
int	execute_job(t_shell *sh)
{
	int	status;

	if (sh->ncmd == 1)
	{
		status = execute_command(&sh->envp, sh);
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, handle_sigquit);
	}
	else
		status = run_pipeline(sh->cmds, sh->ncmd, sh);
	return (status);
}
