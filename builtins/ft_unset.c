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

int	ft_unset(char **av, char ***envp, t_shell *sh)
{
	int	i;

	i = 0;
	if (!av || !*av || ft_arrlen(av) == 1)
		return (0);
	while (av[++i])
	{
		if (!is_valid_identifier(av[i]))
			continue ;
		(*envp) = unset_env_value((*envp), av[i], sh);
	}
	return (0);
}
