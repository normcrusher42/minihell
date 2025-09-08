/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 19:37:17 by nanasser          #+#    #+#             */
/*   Updated: 2025/09/04 19:37:17 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// A simple remake of 'env'. Prints all environment variables.
int	ft_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		ft_putendl_fd(envp[i++], 1);
	return (0);
}
