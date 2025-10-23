/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_fds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 02:35:23 by nanasser          #+#    #+#             */
/*   Updated: 2025/10/21 02:35:23 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* One of the following was done by @Leen && rest by @Nasser */
//		check_pipes - Leen
//		hd_exit_status - Nasser
//		close_cmds_heredoc_fds - Nasser
//		error_return_and_close_fds - Nasser

#include "minishell.h"

// Handles the FD setup for a pipeline of commands.
void	check_pipes(t_pipeinfo *p)
{
	if (p->prev_fd != -1)
	{
		dup2(p->prev_fd, STDIN_FILENO);
		close(p->prev_fd);
	}
	if (p->i < p->n - 1)
	{
		close(p->pipefd[0]);
		dup2(p->pipefd[1], STDOUT_FILENO);
		close(p->pipefd[1]);
	}
}

int	hd_exit_status(int status, int fds[2])
{
	if (WEXITSTATUS(status) == 130)
	{
		close(fds[0]);
		return (1);
	}
	return (0);
}

// Close heredoc FDs after processing but before running the pipeline
void	close_cmds_heredoc_fds(int n, t_cmd *cmds)
{
	int	i;
	int	j;

	i = 0;
	while (i < n)
	{
		j = 0;
		while (j < cmds[i].redir_count)
		{
			if (cmds[i].redirs[j].type == R_HEREDOC)
			{
				if (cmds[i].redirs[j].fd > 2)
					close(cmds[i].redirs[j].fd);
				cmds[i].redirs[j].fd = -1;
			}
			j++;
		}
		i++;
	}
}

int	error_return_and_close_fds(const char *msg, int n, t_shell *sh)
{
	perror(msg);
	close_cmds_heredoc_fds(n, sh->cmds);
	return (1);
}
