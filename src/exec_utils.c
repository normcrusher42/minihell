/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 15:56:42 by nanasser          #+#    #+#             */
/*   Updated: 2025/10/10 16:06:23 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
