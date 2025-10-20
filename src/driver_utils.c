/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   driver_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 18:47:58 by nanasser          #+#    #+#             */
/*   Updated: 2025/10/20 03:56:51 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Most of the following were done by @Leen && one by @Nasser */
//		handle_parent - Leen
//		update_exit_status - Nasser
//		create_pipe_and_fork - Leen
//		join_path - Leen
//		resolve_path - Leen

#include "minishell.h"

// Handles parent-side pipe and file descriptor management.
void	handle_parent(t_pipeinfo *p)
{
	if (p->prev_fd != -1)
		close(p->prev_fd);
	if (p->i < p->n - 1)
	{
		close(p->pipefd[1]);
		p->prev_fd = p->pipefd[0];
	}
}

// Updates the shell's exit status based on the child's termination status.
void	update_exit_status(t_shell *sh, int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", 2);
		sh->ex_st = 128 + WTERMSIG(status);
	}
	else if (WIFEXITED(status))
		sh->ex_st = WEXITSTATUS(status);
}

// Creates a pipe if needed and forks a new process.
int	create_pipe_and_fork(t_pipeinfo *p, int n, t_shell *sh, pid_t *pid)
{
	if (p->i < n - 1 && pipe(p->pipefd) == -1)
	{
		perror("pipe");
		sh->ex_st = 1;
		return (1);
	}
	*pid = fork();
	if (*pid == -1)
	{
		perror("fork");
		sh->ex_st = 1;
		return (1);
	}
	return (0);
}

// Increases or initializes the SHLVL environment variable.
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

// Resolves the full path of a command using the PATH environment variable.
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
