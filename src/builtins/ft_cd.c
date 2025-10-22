/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 04:41:23 by nanasser          #+#    #+#             */
/*   Updated: 2025/09/29 04:41:23 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* The one following was done by @Nasser */
//	   return_cd_null_error
//	   return_cd_error
//	   ft_cd

#include "minishell.h"

// Returns NULL after printing the provided message
static char	*return_cd_null_error(char *msg)
{
	ft_putendl_fd(msg, 2);
	return (NULL);
}

// Returns an error code after printing the provided message
static int	return_cd_error(char *msg)
{
	ft_putendl_fd(msg, 2);
	return (1);
}

// Handles special path cases for cd (~ and -)
static char	*resolve_cd_path(char *arg, char **envp)
{
	char	*home;
	char	*oldpwd;

	home = get_env_value(envp, "HOME");
	oldpwd = get_env_value(envp, "OLDPWD");
	if (!arg)
	{
		if (!home)
			return (return_cd_null_error("-miniOdy: cd: HOME not set"));
		return (ft_strdup(home));
	}
	else if (!ft_strcmp(arg, "-"))
	{
		if (!oldpwd)
			return (return_cd_null_error("-miniOdy: cd: OLDPWD not set"));
		ft_putendl_fd(oldpwd, 1);
		return (ft_strdup(oldpwd));
	}
	else if (arg[0] == '~')
	{
		if (!home)
			return (return_cd_null_error("-miniOdy: cd: HOME not set"));
		return (ft_strjoin(home, arg + 1));
	}
	return (ft_strdup(arg));
}

// A simple remake of 'cd' that changes the current working directory
// and updates PWD and OLDPWD environment variables accordingly.
int	ft_cd(char **av, char ***envp)
{
	char	*path;
	char	*old_path;
	char	cwd[1024];

	if (ft_arrlen(av) > 2)
		return (return_cd_error("-miniOdy: cd: too many arguments"));
	old_path = get_env_value(*envp, "PWD");
	path = resolve_cd_path(av[1], *envp);
	if (!path)
		return (1);
	if (chdir(path) != 0)
	{
		ft_putstr_fd("-miniOdy: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putendl_fd(": No such file or directory", 2);
		free(path);
		return (1);
	}
	if (old_path)
		set_env_value(envp, "OLDPWD", old_path, 0);
	if (getcwd(cwd, sizeof(cwd)))
		set_env_value(envp, "PWD", cwd, 0);
	free(path);
	return (0);
}
