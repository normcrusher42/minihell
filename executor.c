/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 15:54:12 by nanasser          #+#    #+#             */
/*   Updated: 2025/08/04 15:54:12 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command(char *cmd, char **env)
{
	pid_t	pid;
	char	*av[2];
	int		status;

	av[0] = cmd;
	av[1] = NULL;
	status = 0;
	if (is_builtin(av[0]))
		return (g_last_status = exec_builtin(av[0], env), (void)0);
	else
	{
		pid = fork();
		execve(cmd, av, env);
		perror(RED "and u faillled!!!" RESET);
		exit(127);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_last_status = WEXITSTATUS(status);
	else
	{
		perror(RED "and u faillled!!!" RESET);
		g_last_status = 1;
	}
}
