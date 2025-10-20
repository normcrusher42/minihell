/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_fds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 02:35:23 by nanasser          #+#    #+#             */
/*   Updated: 2025/10/21 02:35:23 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* The one following was done by @Leen */
//	   close_cmds_heredoc_fds

#include "minishell.h"

// Close heredoc FDs after processing but before running the pipeline
void	close_cmds_heredoc_fds(int n, t_cmd *cmds)
{
	int	i;
	int	j;

	i = 0;
	while (i < n)
	{
		j = 0;
		while (j < cmds[i].redir_count)
		{
			if (cmds[i].redirs[j].type == R_HEREDOC)
			{
				if (cmds[i].redirs[j].fd > 2)
					close(cmds[i].redirs[j].fd);
				cmds[i].redirs[j].fd = -1;
			}
			j++;
		}
		i++;
	}
}
