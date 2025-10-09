/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 20:38:44 by nanasser          #+#    #+#             */
/*   Updated: 2025/10/09 19:30:56 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Duplicates the passed environment variables into the shell struct.
char	**dup_env(char **envp)
{
	char	**new_envp;
	int		count;

	count = ft_arrlen(envp);
	new_envp = malloc(sizeof(char *) * (count + 1));
	if (!new_envp)
		return (NULL);
	count = -1;
	while (envp[++count])
		new_envp[count] = ft_strdup(envp[count]);
	new_envp[count] = NULL;
	return (new_envp);
}

// Reallocates and copies from the old env array to the new.
char	**realloc_env(char **envp, int extra)
{
	int		i;
	char	**new_envp;

	new_envp = ft_calloc(ft_arrlen(envp) + extra + 1, sizeof(char *));
	if (!new_envp)
		return (NULL);
	i = -1;
	while (envp[++i])
		new_envp[i] = envp[i];
	return (new_envp);
}
