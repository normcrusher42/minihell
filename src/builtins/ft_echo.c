/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 04:41:28 by nanasser          #+#    #+#             */
/*   Updated: 2025/09/29 04:41:28 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* The one following was done by @Nasser */
//     is_n_flag
//	   ft_echo

#include "minishell.h"

// Helper to check if a string is a valid -n flag (like -n, -nn, -nnn...)
static int	is_n_flag(const char *s)
{
	int	i;

	if (!s || s[0] != '-' || s[1] != 'n')
		return (0);
	i = 1;
	while (s[++i])
	{
		if (s[i] != 'n')
			return (0);
	}
	return (1);
}

// A simple remake of 'echo' with the -n flag included. Prints input arguments.
int	ft_echo(char **av)
{
	int	i;
	int	nl_flag;

	i = 1;
	nl_flag = 1;
	while (av[i] && is_n_flag(av[i]))
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
