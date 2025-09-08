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
	if (!ft_strncmp(cmd, "echo", ft_strlen("echo")))
		return (1);
	if (!ft_strncmp(cmd, "pwd", ft_strlen("pwd")))
		return (1);
	if (!ft_strncmp(cmd, "env", ft_strlen("env")))
		return (1);
	if (!ft_strncmp(cmd, "cd", ft_strlen("cd")))
		return (1);
	if (!ft_strncmp(cmd, "export", ft_strlen("export")))
		return (1);
	if (!ft_strncmp(cmd, "unset", ft_strlen("unset")))
		return (1);
	if (!ft_strncmp(cmd, "exit", ft_strlen("exit")))
		return (1);
	return (0);
}

// The built-ins executor based on the passed argument.
int	exec_builtin(char **av, char ***envp)
{
	if (!ft_strncmp(av[0], "echo", ft_strlen("echo")))
		return (ft_echo(av));
	if (!ft_strncmp(av[0], "pwd", ft_strlen("pwd")))
		return (ft_pwd());
	if (!ft_strncmp(av[0], "env", ft_strlen("env")))
		return (ft_env(*envp));
	return (1);
}

void	execute_command(char *cmd, char **env, t_shell *sh)
{
	pid_t	pid;
	char	*av[2];
	int		status;

	(void)sh;
	av[0] = cmd;
	av[1] = NULL;
	status = 0;
	if (is_builtin(av[0]))
		return (g_last_status = exec_builtin(av, &env), (void)0);
	pid = fork();
	if (pid == 0)
	{
		execve(cmd, av, env);
		perror(RED "sm error occured idk" RESET);
		exit(127);
	}
	if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_last_status = WEXITSTATUS(status);
	}
	else
		return (perror(RED "Fork Error" RESET), g_last_status = 1, (void)0);
}
