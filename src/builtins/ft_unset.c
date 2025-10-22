/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 04:42:00 by nanasser          #+#    #+#             */
/*   Updated: 2025/09/29 04:42:00 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* The entire following was done by @Nasser */
//	   unset_error
//	   is_valid_identifier
//	   ft_unset

#include "minishell.h"

// Error message for bad export values (whilst pointing to the bad argument).
static int	unset_error(char *arg)
{
	ft_putstr_fd("miniOdy: unset: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd("': not a valid identifier", 2);
	return (1);
}

// Validity identifier for the first character of the key.
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

// Unsets one or more environment variables from the passed arguments.
int	ft_unset(char **av, char ***envp)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	if (!av || !*av || ft_arrlen(av) == 1)
		return (0);
	while (av[++i])
	{
		if (!is_valid_identifier(av[i]))
		{
			status = unset_error(av[i]);
			continue ;
		}
		(*envp) = unset_env_value((*envp), av[i]);
	}
	return (status);
}
