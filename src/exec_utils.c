/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 15:56:42 by nanasser          #+#    #+#             */
/*   Updated: 2025/10/20 03:28:32 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* The entire following was done by @Nasser */
//	   ft_strcmp
//	   try_direct_exec
//	   init_and_exec_builtins
//	   exec_external
//	   push_word

#include "minishell.h"

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

// Tries to execute a command directly, handling errors appropriately.
void	try_direct_exec(char **av, char ***env, t_shell *sh)
{
	struct stat	st;

	(void)sh;
	execve(av[0], av, *env);
	ft_putstr_fd("miniOdy: ", 2);
	if (stat(av[0], &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_putstr_fd(av[0], 2);
		ft_putendl_fd(": Is a directory", 2);
		call_janitor(sh);
		exit(126);
	}
	perror(av[0]);
	call_janitor(sh);
	if (errno == EACCES)
		exit(126);
	exit(127);
}

// Initializes and executes built-in commands with redirections.
int	init_and_exec_builtins(char **av, char ***env, t_shell *sh)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (!apply_redirections(sh->cmds, sh))
		sh->ex_st = exec_builtin(av, env, sh);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return (sh->ex_st);
}

// Try executing a command directly or via PATH
void	exec_external(t_shell *sh, char **av, char ***env)
{
	t_exec_vars	vars;

	if (ft_strchr(av[0], '/'))
		try_direct_exec(av, env, sh);
	else
	{
		vars.path_env = get_env_value(*env, "PATH");
		if (vars.path_env)
		{
			vars.paths = ft_split(vars.path_env, ':');
			vars.i = -1;
			while (vars.paths && vars.paths[++vars.i])
			{
				vars.full = ft_strjoin3(vars.paths[vars.i], "/", av[0]);
				execve(vars.full, av, *env);
				free(vars.full);
			}
		}
		free_arr(&vars.paths, NO);
		ft_putstr_fd(av[0], 2);
		ft_putendl_fd(": command not found", 2);
		call_janitor(sh);
		exit(127);
	}
}

// Adds a word to the command's argument list.
int	push_word(t_cmd *c, const char *w)
{
	char	**nv;
	int		i;

	nv = (char **)malloc(sizeof(char *) * (c->ac + 2));
	if (!nv)
		return (0);
	i = -1;
	while (++i < c->ac)
		nv[i] = c->av[i];
	nv[i] = ft_strdup(w);
	if (!nv[i])
		return (free(nv), 0);
	nv[i + 1] = NULL;
	free(c->av);
	c->av = nv;
	c->ac++;
	return (1);
}
