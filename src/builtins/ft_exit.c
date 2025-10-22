/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 04:41:41 by nanasser          #+#    #+#             */
/*   Updated: 2025/09/29 04:41:41 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* The entire following was done by @Nasser */
//	   parse_sign
//	   ft_atoll
//	   ft_exit

#include "minishell.h"

// Checks if the string is numeric (with optional leading + or -).
int	parse_sign(const char *str, int *sign)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			*sign = -1;
		i++;
	}
	return (i);
}

// Self-explanatory. But with long long max.
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
	i += parse_sign(str + i, &sign);
	while (ft_isdigit(str[i]))
	{
		if (result > ((unsigned long long)LLONG_MAX / 10) || (result
				== (unsigned long long)LLONG_MAX / 10
				&& (unsigned long long)(str[i] - '0')
			> (unsigned long long)(LLONG_MAX % 10) + (sign == -1)))
			return (0);
		result = (result * 10) + (str[i++] - '0');
	}
	*code = (long long)(sign * result);
	return (1);
}

// A kinda-simple remake of 'exit'. Exits the program with an exit code.
int	ft_exit(char **av, t_shell *sh)
{
	long long	code;

	ft_putendl_fd("exit", 1);
	if (ft_arrlen(av) > 2)
	{
		ft_putendl_fd("miniOdy: exit: too many arguments", 2);
		return (2);
	}
	else if (ft_arrlen(av) < 2)
	{
		call_janitor(sh);
		exit(sh->ex_st);
	}
	if (!is_numeric(av[1]) || !ft_atoll(av[1], &code))
	{
		ft_putendl_fd("miniOdy: exit: numeric argument required", 2);
		call_janitor(sh);
		exit(2);
	}
	call_janitor(sh);
	exit((unsigned char)code);
	return (0);
}
