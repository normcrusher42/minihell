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

/* The entire following was done by @Nasser */
//	   write_heredoc_line
//	   should_break_heredoc
//	   handle_heredoc

#include "minishell.h"

// Writes a line to the heredoc file descriptor, expanding variables if needed.
static int	write_heredoc_line(int fd, char *line, t_shell *sh)
{
	char	*expanded;

	expanded = NULL;
	if (!line)
		return (1);
	if (sh->is_quoted)
		expanded = ft_strdup(line);
	else
		expanded = dollar_expander(line, sh->envp, sh);
	if (!expanded)
		return (1);
	ft_putendl_fd(expanded, fd);
	free(expanded);
	if (sh->is_quoted)
		free(line);
	return (0);
}

// Determines if the heredoc input should stop based on the delimiter.
int	should_break_heredoc(char *line, t_redir *redir)
{
	if (!line)
	{
		ft_putendl_fd("miniOdy: warning: here-document delimited by EOF", 2);
		return (1);
	}
	if (ft_strcmp(line, redir->arg) == 0)
	{
		free(line);
		return (1);
	}
	return (0);
}

// Handles here-document input until the delimiter is reached.
int	handle_heredoc(t_redir *redir, t_shell *sh)
{
	int		fd[2];
	char	*line;

	if (pipe(fd) == -1)
	{
		perror("miniOdy: pipe error");
		return (1);
	}
	sh->in_heredoc = YES;
	while (1)
	{
		line = readline("> ");
		if (should_break_heredoc(line, redir))
			break ;
		if (write_heredoc_line(fd[1], line, sh))
			break ;
	}
	close(fd[1]);
	redir->fd = fd[0];
	sh->in_heredoc = NO;
	return (0);
}
