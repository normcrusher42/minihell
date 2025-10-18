/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsahloul <lsahloul@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 19:00:21 by lsahloul          #+#    #+#             */
/*   Updated: 2025/10/18 19:01:37 by lsahloul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
static void	run_child(t_cmd *cmd, t_shell *sh, t_pipeinfo *p)
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

// Handles parent-side pipe and file descriptor management.
static void	handle_parent(t_pipeinfo *p)
{
	if (p->prev_fd != -1)
		close(p->prev_fd);
	if (p->i < p->n - 1)
	{
		close(p->pipefd[1]);
		p->prev_fd = p->pipefd[0];
	}
}

static int	setup_heredocs(t_cmd *cmds, int n, t_shell *sh)
{
	int	cmd_i;
	int	redir_i;

	cmd_i = -1;
	while (++cmd_i < n)
	{
		redir_i = -1;
		while (++redir_i < cmds[cmd_i].redir_count)
			if (cmds[cmd_i].redirs[redir_i].type == R_HEREDOC)
				if (handle_heredoc(&cmds[cmd_i].redirs[redir_i], sh) == -1)
					return (130);
	}
	return (0);
}
