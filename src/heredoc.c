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

static int	handle_heredoc_eof(void)
{
	ft_putendl_fd("miniOdy: warning: here-document delimited", 2);
	ft_putendl_fd("by end-of-file", 2);
	return (1);
}

static int	setup_heredoc(int fd[2])
{
	if (pipe(fd) == -1)
	{
		perror("miniOdy: pipe");
		return (-1);
	}
	return (0);
}

static int	read_heredoc(int fd[2], t_redir *redir, t_shell *sh)
{
	char	*line;

	sh->in_heredoc = YES;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			handle_heredoc_eof();
			break ;
		}
		if (ft_strcmp(line, redir->arg) == 0)
		{
			free(line);
			break ;
		}
		write_heredoc_line(fd[1], line, sh);
		free(line);
	}
	return (0);
}

int	handle_heredoc(t_redir *redir, t_shell *sh)
{
	int	fd[2];

	if (setup_heredoc(fd) == -1)
		return (-1);
	if (read_heredoc(fd, redir, sh) == -1)
	{
		close(fd[0]);
		close(fd[1]);
		return (-1);
	}
	close(fd[1]);
	redir->fd = fd[0];
	sh->in_heredoc = NO;
	return (0);
}
