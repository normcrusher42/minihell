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

/* The entire following was done by @Nasser */
//	   handle_execution
//	   handle_heredoc_redirections
//	   close_heredoc_fds
//	   handle_child_status
//	   execute_command

#include "minishell.h"

// Runs inside the child process
static void	handle_execution(t_shell *sh, char ***env)
{
	int	status;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	sh->is_child = YES;
	if (apply_redirections(sh->cmds, sh))
	{
		call_janitor(sh);
		exit(1);
	}
	if (!sh->cmds->av || !sh->cmds->av[0])
	{
		call_janitor(sh);
		exit(0);
	}
	if (sh->cmds->av && is_builtin(sh->cmds->av[0]))
	{
		status = exec_builtin(sh->cmds->av, &sh->envp, sh);
		call_janitor(sh);
		exit(status);
	}
	exec_external(sh, sh->cmds->av, env);
}

// Closes all heredoc file descriptors in the command struct.
static void	close_heredoc_fds(t_shell *sh)
{
	int	i;

	i = -1;
	while (++i < sh->cmds->redir_count)
	{
		if (sh->cmds->redirs[i].type == R_HEREDOC)
			close(sh->cmds->redirs[i].fd);
	}
}

// Handles here-document redirections before executing the command.
static int	handle_heredoc_redirections(t_shell *sh)
{
	int	i;

	i = -1;
	while (++i < sh->cmds->redir_count)
	{
		if (sh->cmds->redirs[i].type == R_HEREDOC)
		{
			if (handle_heredoc(&sh->cmds->redirs[i], sh) == 1)
			{
				close_heredoc_fds(sh);
				return (130);
			}
		}
	}
	close_heredoc_fds(sh);
	return (0);
}

// Handles the status of the child process after execution.
static int	handle_child_status(int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			ft_putstr_fd("Quit (core dumped)\n", 2);
		return (128 + WTERMSIG(status));
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

// Executes a command, handling built-ins and external commands.
int	execute_command(char ***env, t_shell *sh)
{
	pid_t	pid;
	int		status;

	if ((!sh->cmds->av || !sh->cmds->av[0]) && sh->cmds->redir_count == 0)
		return (0);
	if (sh->cmds->av && is_builtin(sh->cmds->av[0]))
		return (init_and_exec_builtins(sh->cmds->av, env, sh));
	if (handle_heredoc_redirections(sh))
		return (130);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == 0)
		handle_execution(sh, env);
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		close_heredoc_fds(sh);
		signal_config();
		return (handle_child_status(status));
	}
	perror(RED "Well well, how did we get here? That's embarassing." RESET);
	call_janitor(sh);
	exit(2);
	return (2);
}
