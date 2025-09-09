/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   driver.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 20:39:18 by nanasser          #+#    #+#             */
/*   Updated: 2025/08/13 20:44:02 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_pipeline(t_cmd *cmds, int n, t_shell *sh)
{
	(void)cmds;
	(void)n;
	(void)sh;
	ft_putendl_fd("pipeline execution not implemented yet", 2);
	return (g_last_status);
}

int	execute_job(t_cmd *cmds, int n, t_shell *sh)
{
	if (n == 1)
		return (execute_command(cmds, sh->envp, sh));
	return (run_pipeline(cmds, n, sh));
}
