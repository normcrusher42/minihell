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

// strncmp except it actually compares full string lengths cs it aint in libft??
int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while ((s1[i] || s2[i]))
	{
		if (s1[i] != s2[i])
			return ((unsigned char) s1[i] - (unsigned char) s2[i]);
		i++;
	}
	return (0);
}

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
		return (ft_pwd());
	if (!ft_strcmp(av[0], "env"))
		return (ft_env(*envp));
	if (!ft_strcmp(av[0], "export"))
		return (ft_export(av, envp));
	if (!ft_strcmp(av[0], "unset"))
		return (ft_unset(av, envp, sh));
	if (!ft_strcmp(av[0], "exit"))
		return (ft_exit(av, sh));
	if (!ft_strcmp(av[0], "cd"))
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
