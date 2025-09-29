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

#include "minishell.h"

// Validity identifier for the first character of the key.
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

// Unsets one or more environment variables from the passed arguments.
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
