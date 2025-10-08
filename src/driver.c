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
int	apply_redirections(t_cmd *cmd)
{
    (void)cmd;
    return (0);
}

static char	*join_path(const char *dir, const char *cmd)
{
	char	*tmp;
	char	*path;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	path = ft_strjoin(tmp, cmd);
	free(tmp);
	return (path);
}

/* Searches PATH env variable for the given command */
char	*resolve_path(char *cmd, char **envp)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	int		i;

	if (!cmd || ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_env = get_env_value(envp, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		full_path = join_path(paths[i], cmd);
		if (access(full_path, X_OK) == 0)
			return (free_arr(&paths, NO), full_path);
		free(full_path);
	}
	free_arr(&paths, NO);
	return (NULL);
}
static void	exec_cmd(t_cmd *cmd, t_shell *sh)
{
	char	*path;

	path = resolve_path(cmd->av[0], sh->envp);
	if (!path)
	{
		ft_putstr_fd(cmd->av[0], 2);
		ft_putendl_fd(": command not found", 2);
		call_janitor(sh);
		exit(127);
	}
	execve(path, cmd->av, sh->envp);
	perror(cmd->av[0]);
	free(path);
	call_janitor(sh);
	exit(127);
}

static void	run_child(t_cmd *cmd, t_shell *sh, t_pipeinfo *p)
{
	if (p->prev_fd != -1)
	{
		dup2(p->prev_fd, STDIN_FILENO);
		close(p->prev_fd);
	}
	if (p->i < p->n - 1)
	{
		close(p->pipefd[0]);
		dup2(p->pipefd[1], STDOUT_FILENO);
		close(p->pipefd[1]);
	}
	if (cmd->redir_count > 0)
		apply_redirections(cmd);
	if (is_builtin(cmd->av[0]))
		exit(exec_builtin(cmd->av, &sh->envp, sh));
	exec_cmd(cmd, sh);
}

static void	handle_parent(t_pipeinfo *p)
{
	if (p->prev_fd != -1)
		close(p->prev_fd);
	if (p->i < p->n - 1)
	{
		close(p->pipefd[1]);
		p->prev_fd = p->pipefd[0];
	}
}

int	run_pipeline(t_cmd *cmds, int n, t_shell *sh)
{
	t_pipeinfo	p;
	pid_t		pid;
	int			status;

	p.prev_fd = -1;
	p.i = -1;
	p.n = n;
	while (++p.i < n)
	{
		if (p.i < n - 1 && pipe(p.pipefd) == -1)
			return (perror("pipe"), g_last_status = 1);
		pid = fork();
		if (pid == -1)
			return (perror("fork"), g_last_status = 1);
		if (pid == 0)
			run_child(&cmds[p.i], sh, &p);
		handle_parent(&p);
	}
	while (wait(&status) > 0)
		if (WIFEXITED(status))
			g_last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_last_status = 128 + WTERMSIG(status);
	return (g_last_status);
}

/* Match minishell.h signature */
int	execute_job(t_cmd *cmds, int n, t_shell *sh)
{
	if (n == 1)
		return (execute_command(&sh->envp, sh));
	return (run_pipeline(cmds, n, sh));
}
