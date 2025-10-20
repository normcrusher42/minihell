/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_pipelines.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 05:06:35 by nanasser          #+#    #+#             */
/*   Updated: 2025/10/20 05:06:35 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* The one following was done by @Leen */
//	   spawn_pipeline_children

#include "minishell.h"

// Spawn children for the pipeline, store their pids in pids.
// Returns 0 on success, non-zero on error (and frees pids when needed).
int	spawn_pipeline_children(t_cmd *cmds, int n, t_shell *sh, t_pipeinfo *p)
{
	pid_t	pid;

	p->i = -1;
	while (++p->i < n)
	{
		if (create_pipe_and_fork(p, n, sh, &pid))
		{
			free(p->pids);
			return (1);
		}
		p->pids[p->i] = pid;
		if (pid == 0)
			run_child(&cmds[p->i], sh, p);
		handle_parent(p);
	}
	p->i = -1;
	return (0);
}
