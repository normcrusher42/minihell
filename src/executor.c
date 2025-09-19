/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 22:06:01 by nanasser          #+#    #+#             */
/*   Updated: 2025/09/04 22:06:01 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_last_status = 0;

// A checker if the argument is a built-in.
int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (!ft_strncmp(cmd, "echo", ft_strlen(cmd)))
		return (1);
	if (!ft_strncmp(cmd, "pwd", ft_strlen(cmd)))
		return (1);
	if (!ft_strncmp(cmd, "env", ft_strlen(cmd)))
		return (1);
	if (!ft_strncmp(cmd, "cd", ft_strlen(cmd)))
		return (1);
	if (!ft_strncmp(cmd, "export", ft_strlen(cmd)))
		return (1);
	if (!ft_strncmp(cmd, "unset", ft_strlen(cmd)))
		return (1);
	if (!ft_strncmp(cmd, "exit", ft_strlen(cmd)))
		return (1);
	return (0);
}

// The built-ins executor based on the passed argument.
int	exec_builtin(char **av, char ***envp, t_shell *sh)
{
	if (!ft_strncmp(av[0], "echo", ft_strlen(av[0])))
		return (ft_echo(av));
	if (!ft_strncmp(av[0], "pwd", ft_strlen(av[0])))
		return (ft_pwd());
	if (!ft_strncmp(av[0], "env", ft_strlen(av[0])))
		return (ft_env(*envp));
	if (!ft_strncmp(av[0], "export", ft_strlen(av[0])))
		return (ft_export(av, envp));
	if (!ft_strncmp(av[0], "unset", ft_strlen(av[0])))
		return (ft_unset(av, envp, sh));
	if (!ft_strncmp(av[0], "exit", ft_strlen(av[0])))
		return (ft_exit(av, sh));
	if (!ft_strncmp(av[0], "cd", ft_strlen(av[0])))
		return (ft_cd(av, envp));
	return (1);
}

// Runs a single cmd passed and checks if its a builtin or a program
int	execute_command(char ***env, t_shell *sh)
{
	pid_t	pid;
	int		status;

	if (!sh->cmds->av || !sh->cmds->av[0])
		return (g_last_status = 0);
	if (is_builtin(sh->cmds->av[0]))
		return (g_last_status = exec_builtin(sh->cmds->av, env, sh));
	pid = fork();
	if (pid == 0)
	{
		execve(sh->cmds->av[0], sh->cmds->av, *env);
		ft_putstr_fd(sh->cmds->av[0], 2);
		ft_putendl_fd(": command not found", 2);
		call_janitor(sh);
		exit(127);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (g_last_status = WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			return (g_last_status = 128 + WTERMSIG(status));
	}
	return (perror(RED "Fork Error" RESET), g_last_status = 1);
}
