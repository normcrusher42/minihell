#include "minishell.h"

// A simple remake of 'env'. Prints all environment variables
int	ft_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		ft_putendl_fd(envp[i++], 1);
	return (0);
}
