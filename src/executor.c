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

// A checker if the argument is a built-in.
int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (!ft_strcmp(cmd, "echo"))
		return (1);
	if (!ft_strcmp(cmd, "pwd"))
		return (1);
	if (!ft_strcmp(cmd, "env"))
		return (1);
	if (!ft_strcmp(cmd, "cd"))
		return (1);
	if (!ft_strcmp(cmd, "export"))
		return (1);
	if (!ft_strcmp(cmd, "unset"))
		return (1);
	if (!ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

// The built-ins executor based on the passed argument.
int	exec_builtin(char **av, char ***envp, t_shell *sh)
{
	if (!ft_strcmp(av[0], "echo"))
		return (ft_echo(av));
	if (!ft_strcmp(av[0], "pwd"))
		return (ft_pwd(sh));
	if (!ft_strcmp(av[0], "env"))
		return (ft_env(*envp));
	if (!ft_strcmp(av[0], "export"))
		return (ft_export(av, envp));
	if (!ft_strcmp(av[0], "unset"))
		return (ft_unset(av, envp));
	if (!ft_strcmp(av[0], "exit"))
		return (ft_exit(av, sh));
	if (!ft_strcmp(av[0], "cd"))
		return (ft_cd(av, envp));
	return (1);
}

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

// Runs a single cmd passed and checks if its a builtin or a program
int	execute_command(char ***env, t_shell *sh)
{
	pid_t	pid;
	int		status;

	if ((!sh->cmds->av || !sh->cmds->av[0]) && sh->cmds->redir_count == 0)
		return (0);
	if (sh->cmds->av && is_builtin(sh->cmds->av[0]))
		return (init_and_exec_builtins(sh->cmds->av, env, sh));
	// Handle heredocs first
	int i = -1;
	while (++i < sh->cmds->redir_count)
	{
		if (sh->cmds->redirs[i].type == R_HEREDOC)
			if (handle_heredoc(&sh->cmds->redirs[i], sh) == -1)
				return (130);
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);  // Parent ignores SIGQUIT
	pid = fork();
	if (pid == 0)
		handle_execution(sh, env);
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		i = -1;
		while (++i < sh->cmds->redir_count)
			if (sh->cmds->redirs[i].type == R_HEREDOC)
				close(sh->cmds->redirs[i].fd);
		sh->is_child = NO;
		// Restore signal handlers after child completes
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
	}
	perror(RED "Well well, how did we get here? That's embarassing." RESET);
	call_janitor(sh);
	exit(2);
	return (2);
}
