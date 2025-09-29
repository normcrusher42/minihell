/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 04:40:39 by nanasser          #+#    #+#             */
/*   Updated: 2025/09/29 04:40:39 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_last_status = 0;

// strncmp except it actually compares full string lengths cs it aint in libft??
int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while ((s1[i] || s2[i]))
	{
		if (s1[i] != s2[i])
			return ((unsigned char) s1[i] - (unsigned char) s2[i]);
		i++;
	}
	return (0);
}

// A checker if the argument is a built-in.
int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (!ft_strcmp(cmd, "echo"))
		return (1);
	if (!ft_strcmp(cmd, "pwd"))
		return (1);
	if (!ft_strcmp(cmd, "env"))
		return (1);
	if (!ft_strcmp(cmd, "cd"))
		return (1);
	if (!ft_strcmp(cmd, "export"))
		return (1);
	if (!ft_strcmp(cmd, "unset"))
		return (1);
	if (!ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

// The built-ins executor based on the passed argument.
int	exec_builtin(char **av, char ***envp, t_shell *sh)
{
	if (!ft_strcmp(av[0], "echo"))
		return (ft_echo(av));
	if (!ft_strcmp(av[0], "pwd"))
		return (ft_pwd());
	if (!ft_strcmp(av[0], "env"))
		return (ft_env(*envp));
	if (!ft_strcmp(av[0], "export"))
		return (ft_export(av, envp));
	if (!ft_strcmp(av[0], "unset"))
		return (ft_unset(av, envp, sh));
	if (!ft_strcmp(av[0], "exit"))
		return (ft_exit(av, sh));
	if (!ft_strcmp(av[0], "cd"))
		return (ft_cd(av, envp));
	return (1);
}

static void	print_exec_error(char *cmd, t_shell *sh)
{
	struct stat	st;

	if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putendl_fd(": Is a directory", 2);
		call_janitor(sh);
		exit(126);
	}
	perror(cmd);
	call_janitor(sh);
	if (errno == EACCES)
		exit(126);
	exit(127);
}

static void	try_direct_exec(char **av, char ***env, t_shell *sh)
{
	execve(av[0], av, *env);
	print_exec_error(av[0], sh);
}

// Try executing a command directly or via PATH
static void	exec_external(t_shell *sh, char **av, char ***env)
{
	t_exec_vars	vars;

	if (ft_strchr(av[0], '/'))
		try_direct_exec(av, env, sh);
	else
	{
			vars.path_env = get_env_value(*env, "PATH");
		if (!vars.path_env)
			vars.path_env = "/bin:/usr/bin";
		vars.paths = ft_split(vars.path_env, ':');
		vars.i = -1;
	while (vars.paths && vars.paths[++vars.i])
	{
		vars.full = ft_strjoin3(vars.paths[vars.i], "/", av[0]);
		execve(vars.full, av, *env);
		free(vars.full);
	}
		free_arr(&vars.paths, NO);
		ft_putstr_fd(av[0], 2);
		ft_putendl_fd(": command not found", 2);
		call_janitor(sh);
		exit(127);
	}
}

// Runs inside the child process
static void	handle_execution(t_shell *sh, char ***env)
{
	if (is_builtin(sh->cmds->av[0]))
		exit(exec_builtin(sh->cmds->av, env, sh));
	else
		exec_external(sh, sh->cmds->av, env);
}

// Runs a single cmd passed and checks if its a builtin or a program
int	execute_command(char ***env, t_shell *sh)
{
	pid_t	pid;
	int		status;

	if (!sh->cmds->av || !sh->cmds->av[0])
		return (g_last_status = 0);
	if (is_builtin(sh->cmds->av[0]))
		return (g_last_status = exec_builtin(sh->cmds->av, env, sh));
	pid = fork();
	if (pid == 0)
		handle_execution(sh, env);
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (g_last_status = WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			return (g_last_status = 128 + WTERMSIG(status));
	}
	perror(RED "Well well, how did we get here? That's embarassing." RESET);
	return (g_last_status = 2);
}
