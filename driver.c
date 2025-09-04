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

int	execute_job(t_cmd *cmds, int n, t_shell *sh)
{
	int	i;

	i = 0;
	while (i < n)
	{
		printf("  Command %d: %s\n", i, cmds[i].av[0]);
	}
	return (0);
}
