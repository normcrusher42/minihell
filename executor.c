/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: team                                           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 15:54:12 by team              #+#    #+#             */
/*   Updated: 2025/08/10 21:00:00 by team             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>

/*
 * Temporary single-command executor (no PATH search yet).
 * - On failure in child: exit 126 (permission) or 127 (not found/other),
 *   and print a standard perror.
 */
void	execute_command(char *cmd, char **env)
{
	pid_t	pid;
	char	*argv_[2];
	int		status;

	if (!cmd)
		return ;
	argv_[0] = cmd;
	argv_[1] = NULL;
	pid = fork();
	if (pid == 0)
	{
		execve(cmd, argv_, env);
		if (errno == EACCES)
			perror("execve");
		else
			perror("execve");
		_exit(errno == EACCES ? 126 : 127);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		(void)status;
	}
	else
		perror("fork");
}
