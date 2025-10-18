/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_core.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsahloul <lsahloul@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 19:09:31 by lsahloul          #+#    #+#             */
/*   Updated: 2025/10/18 19:10:56 by lsahloul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Try executing a command directly or via PATH
static void	exec_external(t_shell *sh, char **av, char ***env)
{
	t_exec_vars	vars;

	if (ft_strchr(av[0], '/'))
		try_direct_exec(av, env, sh);
	else
	{
		vars.path_env = get_env_value(*env, "PATH");
		if (vars.path_env)
		{
			vars.paths = ft_split(vars.path_env, ':');
			vars.i = -1;
			while (vars.paths && vars.paths[++vars.i])
			{
				vars.full = ft_strjoin3(vars.paths[vars.i], "/", av[0]);
				execve(vars.full, av, *env);
				free(vars.full);
			}
		}
		free_arr(&vars.paths, NO);
		ft_putstr_fd(av[0], 2);
		ft_putendl_fd(": command not found", 2);
		call_janitor(sh);
		exit(127);
	}
}

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

static int	handle_parent_process(pid_t pid, t_shell *sh)
{
	int	status;
	int	i;

	waitpid(pid, &status, 0);
	i = -1;
	while (++i < sh->cmds->redir_count)
		if (sh->cmds->redirs[i].type == R_HEREDOC)
			close(sh->cmds->redirs[i].fd);
	sh->is_child = NO;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
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
