#include "minishell.h"

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
