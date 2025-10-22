/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 15:57:06 by nanasser          #+#    #+#             */
/*   Updated: 2025/10/20 05:05:42 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* The entire following was done by @Nasser */
//	   realloc_env
//	   dup_env

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
