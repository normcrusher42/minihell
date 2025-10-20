/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 04:41:33 by nanasser          #+#    #+#             */
/*   Updated: 2025/09/29 04:41:33 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* The one following was done by @Nasser */
//	   ft_env

#include "minishell.h"

// A simple remake of 'env'. Prints all environment variables.
int	ft_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strchr(envp[i], '='))
			ft_putendl_fd(envp[i], 1);
		i++;
	}
	return (0);
}
