/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 01:15:32 by nanasser          #+#    #+#             */
/*   Updated: 2025/10/14 00:02:18 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	write_heredoc_line(int fd, char *line, t_shell *sh)
{
	char	*expanded;

	expanded = NULL;
	if (sh->is_quoted)
		expanded = ft_strdup(line);
	else
		expanded = dollar_expander(line, sh->envp, sh);
	ft_putendl_fd(expanded, fd);
	if (expanded)
        free(expanded);
}

int	handle_heredoc(t_redir *redir, t_shell *sh)
{
	int		fd[2];
	char	*line;

	if (pipe(fd) == -1)
	{
		perror("minishell: pipe");
		return (-1);
	}
	sh->in_heredoc = YES;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putendl_fd("minishell: warning: here-document delimited by EOF", 2);
			break ;
		}
		if (ft_strcmp(line, redir->arg) == 0)
		{
			free(line);
			break ;
		}
		write_heredoc_line(fd[1], line, sh);
	}
	close(fd[1]);
	redir->fd = fd[0];
	sh->in_heredoc = NO;
	return (0);
}
