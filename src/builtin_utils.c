/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 18:15:46 by nanasser          #+#    #+#             */
/*   Updated: 2025/10/20 03:43:48 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* The entire following was done by @Nasser */
//	   is_builtin
//	   exec_builtin

#include "minishell.h"

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
		return (ft_pwd(sh));
	if (!ft_strcmp(av[0], "env"))
		return (ft_env(*envp));
	if (!ft_strcmp(av[0], "export"))
		return (ft_export(av, envp));
	if (!ft_strcmp(av[0], "unset"))
		return (ft_unset(av, envp));
	if (!ft_strcmp(av[0], "exit"))
		return (ft_exit(av, sh));
	if (!ft_strcmp(av[0], "cd"))
		return (ft_cd(av, envp));
	return (1);
}
