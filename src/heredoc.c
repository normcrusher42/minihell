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
//	   catch_sigint_heredoc
//	   write_heredoc_line
//	   should_break_heredoc
//	   heredoc_child
//	   handle_heredoc

#include "minishell.h"

extern volatile sig_atomic_t	g_signal;

static void	catch_sigint_heredoc(int signum)
{
	if (signum == SIGINT)
	{
		g_signal = SIGINT;
		write(STDIN_FILENO, "\n\r", 1);
		close(STDIN_FILENO);
	}
}

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
static int	should_break_heredoc(char *line, t_redir *redir)
{
	if (!line && g_signal == SIGINT)
		return (1);
	if (!line)
	{
		ft_putstr_fd("miniOdy: warning: here-document delimited by EOF ", 2);
		ft_putstr_fd("(wanted `", 2);
		ft_putstr_fd(redir->arg, 2);
		ft_putendl_fd("')", 2);
		return (1);
	}
	if (ft_strcmp(line, redir->arg) == 0)
	{
		free(line);
		return (1);
	}
	return (0);
}

// The child process that handles heredoc input
static void	heredoc_child(int fds[2], t_redir *redir, t_shell *sh)
{
	char	*line;

	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, catch_sigint_heredoc);
	close(fds[0]);
	sh->in_heredoc = YES;
	while (1)
	{
		line = readline("> ");
		if (should_break_heredoc(line, redir))
			break ;
		if (write_heredoc_line(fds[1], line, sh))
			break ;
	}
	close(fds[1]);
	call_janitor(sh);
	if (g_signal == SIGINT)
		exit(g_signal + 128);
	exit(0);
}

// The main heredoc handler in the parent shell
int	handle_heredoc(t_redir *redir, t_shell *sh)
{
	int		fds[2];
	int		pid;
	int		status;

	if (pipe(fds) == -1)
	{
		perror("miniOdy: pipe error");
		return (1);
	}
	sh->in_heredoc = YES;
	pid = fork();
	if (pid == -1)
	{
		perror("miniOdy: fork error");
		return (1);
	}
	else if (pid == 0)
		heredoc_child(fds, redir, sh);
	signal(SIGINT, SIG_IGN);
	close(fds[1]);
	waitpid(pid, &status, 0);
	sh->in_heredoc = NO;
	redir->fd = fds[0];
	init_signals();
	return (hd_exit_status(status, fds));
}
