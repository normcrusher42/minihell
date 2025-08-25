/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:09:19 by nanasser          #+#    #+#             */
/*   Updated: 2025/08/25 21:08:49 by nanasser         ###   ########.fr       */
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

int	is_valid_identifier(const char *s)
{
	int	i;

	if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}



int	ft_exit(char **av, t_shell *sh)
{
	long	code;

	ft_putendl_fd("exit miniOdy", 1);
	if (ft_arrlen(av) > 2)
		return (perror("mOdy: exit: too many arguments"), 1);
	else if (ft_arrlen(av) < 2)
	{
		call_janitor(sh);
		exit(g_last_status);
	}
	code = ft_atoi(av[1]);
	if (!is_numeric(av[1]) || code > 256u)
	{
		perror("mOdy: exit: numeric argument required");
		call_janitor(sh);
		exit(2);
	}
	return (0);
}

int	ft_unset(char **av, char ***envp, t_shell *sh)
{
	int	i;

	i = 0;
	if (ft_arrlen(av) == 1)
		return (0);
	while (av[++i])
		if (!is_valid_identifier(av[i]))
			continue ;
	(*envp) = unset_env_value((*envp), av[i], sh);
	return (0);
}

// A simple remake of 'export'. Prints 
int	ft_export(char **av, char ***envp)
{
	int		i;
	char	*equal;

	if (ft_arrlen(av) == 1)
		return (ft_env(*envp), 0);
	i = 0;
	while (av[++i])
	{
		if (!is_valid_identifier(av[i]))
		{
			ft_putstr_fd("mOdy: export: `", 2);
			ft_putstr_fd(av[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			continue ;
		}
		equal = ft_strchr(av[i], '=');
		if (equal)
			set_env_value(envp, av[i], equal + 1);
		else if (!get_env_value(*envp, av[i]))
			set_env_value(envp, av[i], "");
	}
	return (0);
}

// A simple remake of 'cd'. Changes current working directory based on input
int	ft_cd(char **av, char **envp)
{
	char	*path;
	char	*old_path;
	char	cwd[1024];

	if (ft_arrlen(av) > 2)
		return (perror("-mOdy: cd: too many arguments"), 1);
	else if (ft_arrlen(av) == 1)
	{
		path = get_env_value(envp, "HOME");
		if (!path)
			return (perror("-mOdy: cd: HOME not set"), 1);
	}
	else
		path = av[1];
	old_path = get_env_value(envp, "PWD");
	if (chdir(path) != 0)
		return (perror("-mOdy: cd: path error"), 1);
	if (old_path)
		set_env_value(&envp, "OLDPWD", old_path);
	if (getcwd(cwd, sizeof(cwd)))
		set_env_value(&envp, "PWD", cwd);
	return (0);
}

// A simple remake of 'env'. Prints all environment variables
int	ft_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		ft_putendl_fd(envp[i++], 1);
	return (0);
}

// A simple remake of 'pwd', prints the current working directory
int	ft_pwd(void)
{
	char	buf[1024];

	if (getcwd(buf, sizeof(buf)))
		return (ft_putendl_fd(buf, 1), 0);
	perror("pwd");
	return (1);
}

// A simple remake of 'echo' with the -n flag included. Prints input arguments
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
