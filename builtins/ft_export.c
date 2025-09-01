#include "minishell.h"

static int	is_valid_identifier(const char *s)
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

int	ft_export_print(char **envp)
{
	int		i;
	char	*equal;

	i = 0;
	while (envp[i])
	{
		equal = ft_strchr(envp[i], '=');
		if (equal)
		{
			*equal = '\0';
			ft_printf("declare -x %s=\"%s\"\n", envp[i], equal + 1);
			*equal = '=';
		}
		else
			ft_printf("declare -x %s\n", envp[i]);
		i++;
	}
	return (0);
}

static void	export_error(char *arg)
{
	ft_putstr_fd("mOdy: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd("': not a valid identifier", 2);
}

static void	export_assign(char ***envp, const char *arg, char *equal)
{
	char	*key;
	char	*value;

	key = ft_substr(arg, 0, equal - arg);
	value = ft_strdup(equal + 1);
	if (key)
		set_env_value(envp, key, value);
	free(key);
	free(value);
}

// A simple remake of 'export'. Prints 
int	ft_export(char **av, char ***envp)
{
	int		i;
	char	*equal;

	if (ft_arrlen(av) == 1)
		return (ft_export_print(*envp));
	i = 0;
	while (av[++i])
	{
		if (!is_valid_identifier(av[i]))
		{
			export_error(av[i]);
			continue ;
		}
		equal = ft_strchr(av[i], '=');
		if (equal)
			export_assign(envp, av[i], equal);
		else
			set_env_value(envp, av[i], NULL);
	}
	return (0);
}
