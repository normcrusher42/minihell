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

/* Half of the following were done by @Nasser && @Leen */
//		exec_cmd - Nasser
//		run_child - Leen
//		process_heredocs - Nasser
//		run_pipeline - Leen
//		execute_job - Nasser && Leen

#include "minishell.h"

// Executes an external command using execve.
static void	exec_cmd(t_cmd *cmd, t_shell *sh)
{
	char	*path;

	path = resolve_path(cmd->av[0], sh->envp);
	if (!path)
	{
		ft_putstr_fd(cmd->av[0], 2);
		ft_putendl_fd(": command not found", 2);
		call_janitor(sh);
		exit(127);
	}
	execve(path, cmd->av, sh->envp);
	perror(cmd->av[0]);
	free(path);
	call_janitor(sh);
	exit(127);
}

// The child process logic for executing a command in a pipeline.
void	run_child(t_cmd *cmd, t_shell *sh, t_pipeinfo *p)
{
	int	status;

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
	if (apply_redirections(cmd, sh))
	{
		call_janitor(sh);
		exit(1);
	}
	if (is_builtin(cmd->av[0]))
	{
		status = exec_builtin(cmd->av, &sh->envp, sh);
		call_janitor(sh);
		exit(status);
	}
	exec_cmd(cmd, sh);
}

// Processes all here-document redirections for the command table.
static int	process_heredocs(t_cmd *cmds, int n, t_shell *sh)
{
	int	cmd_i;
	int	redir_i;

	cmd_i = -1;
	while (++cmd_i < n)
	{
		redir_i = -1;
		while (++redir_i < cmds[cmd_i].redir_count)
		{
			if (cmds[cmd_i].redirs[redir_i].type == R_HEREDOC)
			{
				if (handle_heredoc(&cmds[cmd_i].redirs[redir_i], sh) == 1)
				{
					close_cmds_heredoc_fds(n, cmds);
					return (130);
				}
			}
		}
	}
	close_cmds_heredoc_fds(n, cmds);
	return (0);
}

// Manages the entire pipeline execution with forking and piping.
int	run_pipeline(t_cmd *cmds, int n, t_shell *sh)
{
	t_pipeinfo	p;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (process_heredocs(cmds, n, sh) == 130)
		return (130);
	p.pids = malloc(sizeof(pid_t) * n);
	if (!p.pids)
	{
		perror("malloc error");
		sh->ex_st = 1;
		return (sh->ex_st);
	}
	p.prev_fd = -1;
	p.n = n;
	if (spawn_pipeline_children(cmds, n, sh, &p) != 0)
		return (sh->ex_st);
	while (++p.i < n)
	{
		waitpid(p.pids[p.i], &p.status, 0);
		if (p.i == n - 1)
			update_exit_status(sh, p.status);
	}
	free(p.pids);
	return (sh->ex_st);
}

// Executes the entire job, either a single command or a pipeline.
int	execute_job(t_shell *sh)
{
	int	status;

	if (sh->ncmd == 1)
	{
		status = execute_command(&sh->envp, sh);
		sh->is_child = NO;
	}
	else
		status = run_pipeline(sh->cmds, sh->ncmd, sh);
	signal_config();
	return (status);
}
