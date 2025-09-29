/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   driver.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 04:40:52 by nanasser          #+#    #+#             */
/*   Updated: 2025/09/29 04:40:52 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_pipeline(t_shell *sh)
{
	(void)sh;
	ft_putendl_fd("pipeline execution not implemented yet", 2);
	return (g_last_status);
}

int	execute_job(t_shell *sh)
{
	if (sh->ncmd == 1)
		return (execute_command(&sh->envp, sh));
	return (run_pipeline(sh));
}
