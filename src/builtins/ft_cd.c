/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 19:37:13 by nanasser          #+#    #+#             */
/*   Updated: 2025/09/04 19:37:13 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// A simple remake of 'cd'. Changes current working directory based on input.
int	ft_cd(char **av, char ***envp)
{
	char	*path;
	char	*old_path;
	char	cwd[1024];

	if (ft_arrlen(av) > 2)
		return (perror("-mOdy: cd: too many arguments"), 1);
	else if (ft_arrlen(av) == 1)
	{
		path = get_env_value(*envp, "HOME");
		if (!path)
			return (perror("-mOdy: cd: HOME not set"), 1);
	}
	else
		path = av[1];
	old_path = get_env_value(*envp, "PWD");
	if (chdir(path) != 0)
		return (perror("-mOdy: cd: path error"), 1);
	if (old_path)
		set_env_value(envp, "OLDPWD", old_path, 0);
	if (getcwd(cwd, sizeof(cwd)))
		set_env_value(envp, "PWD", cwd, 0);
	return (0);
}
