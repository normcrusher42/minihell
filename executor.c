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

	pid = fork();
	av[0] = cmd;
	av[1] = NULL;
	status = 0;
	if (pid == 0)
	{
		execve(cmd, av, env);
		perror(RED "and u faillled!!!" RESET);
		exit(127);
	}
	else if (pid > 0)
		waitpid(pid, &status, 0);
	else
		perror(RED "and u faillled!!!" RESET);
}
