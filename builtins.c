/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:09:19 by nanasser          #+#    #+#             */
/*   Updated: 2025/08/16 15:49:38 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// A checker if the argument is a built-in
int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (!ft_strncmp(cmd, "echo", ft_strlen("echo")))
		return (1);
	if (!ft_strncmp(cmd, "pwd", ft_strlen("pwd")))
		return (1);
	if (!ft_strncmp(cmd, "env", ft_strlen("env")))
		return (1);
	if (!ft_strncmp(cmd, "cd", ft_strlen("cd")))
		return (1);
	if (!ft_strncmp(cmd, "export", ft_strlen("export")))
		return (1);
	if (!ft_strncmp(cmd, "unset", ft_strlen("unset")))
		return (1);
	if (!ft_strncmp(cmd, "exit", ft_strlen("exit")))
		return (1);
	return (0);
}

// A simple remake of 'env'. Prints all environment variables
int	ft_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		ft_putendl_fd(envp[i], 1);
		i++;
	}
	return (0);
}

// A simple remake of 'pwd'
int	ft_pwd(void)
{
	char	buf[1024];

	if (getcwd(buf, sizeof(buf)))
	{
		ft_putendl_fd(buf, 1);
		return (0);
	}
	perror("pwd");
	return (1);
}

// A simple remake of 'echo' with the -n flag included.
int	ft_echo(char **av)
{
	int	i;
	int	nl_flag;

	i = 1;
	nl_flag = 1;
	if (av[1] && !ft_strncmp(av[1], "-n", ft_strlen("-n")))
	{
		nl_flag = 0;
		i++;
	}
	while (av[i])
	{
		ft_putstr_fd(av[i], 1);
		if (av[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (nl_flag)
		ft_putchar_fd('\n', 1);
	return (0);
}

// The built-ins executor based on the passed argument
int	exec_builtin(char **av, char ***envp)
{
	if (!ft_strncmp(av[0], "echo", ft_strlen("echo")))
		return (ft_echo(av));
	if (!ft_strncmp(av[0], "pwd", ft_strlen("pwd")))
		return (ft_pwd());
	if (!ft_strncmp(av[0], "env", ft_strlen("env")))
		return (ft_env(*envp));
	return (1);
}
