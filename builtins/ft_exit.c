#include "minishell.h"

int	ft_atoll(const char *str, long long *code)
{
	int						i;
	int						sign;
	unsigned long long		result;

	i = 0;
	sign = 1;
	result = 0;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (ft_isdigit(str[i]))
	{
		if (result > ((unsigned long long)LLONG_MAX / 10) || (result == \
					(unsigned long long)LLONG_MAX / 10
				&& (unsigned long long)str[i]
				> (unsigned long long)(LLONG_MAX % 10) + (sign == -1)))
			return (0);
		result = (result * 10) + (str[i++] - '0');
	}
	return (*code = (long long)(sign * result), 1);
}

int	ft_exit(char **av, t_shell *sh)
{
	long long	code;

	ft_putendl_fd("exit miniOdy", 1);
	if (ft_arrlen(av) > 2)
		return (perror("mOdy: exit: too many arguments"), 1);
	else if (ft_arrlen(av) < 2)
	{
		call_janitor(sh);
		exit(g_last_status);
	}
	if (!is_numeric(av[1]) || !ft_atoll(av[1], &code))
	{
		perror("mOdy: exit: numeric argument required");
		call_janitor(sh);
		exit(2);
	}
	call_janitor(sh);
	exit((unsigned char)code);
	return (0);
}
