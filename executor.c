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

int	g_last_status = 0;

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
