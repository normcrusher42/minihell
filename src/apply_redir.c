/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 15:56:10 by nanasser          #+#    #+#             */
/*   Updated: 2025/10/10 15:56:10 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Opens a file based on redirection type */
static int	open_redirection(t_redir *redir)
{
	int	fd;

	fd = -1;
	if (redir->type == R_IN)
		fd = open(redir->arg, O_RDONLY);
	else if (redir->type == R_OUT)
		fd = open(redir->arg, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == R_APP)
		fd = open(redir->arg, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (fd);
}

/* Applies one redirection (input or output) */
static int	apply_single_redirection(t_redir *redir, t_shell *sh)
{
	int	fd;

	fd = open_redirection(redir);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(redir->arg);
		if (!sh->is_child)
			return (1);
		call_janitor(sh);
		exit(1);
	}
	if (redir->type == R_IN)
		dup2(fd, STDIN_FILENO);
	else if (redir->type == R_OUT || redir->type == R_APP)
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

/* Main entry point for redirection handling */
int	apply_redirections(t_cmd *cmd, t_shell *sh)
{
	int	i;

	i = -1;
	while (++i < cmd->redir_count)
	{
		if (cmd->redirs[i].type == R_HEREDOC)
		{
			dup2(cmd->redirs[i].fd, STDIN_FILENO);
			close(cmd->redirs[i].fd);
			continue ;
		}
		if (apply_single_redirection(&cmd->redirs[i], sh))
			return (1);
	}
	return (0);
}
