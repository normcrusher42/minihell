/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:07:59 by nanasser          #+#    #+#             */
/*   Updated: 2025/07/31 21:35:27 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Frees the passed double array pointer and its content (with a reuse switch) 
void	free_arr(char ***arr, bool reuse)
{
	int	i;

	if (!arr || !*arr)
		return ;
	i = -1;
	while ((*arr)[++i])
	{
		free((*arr)[i]);
		(*arr)[i] = NULL;
	}
	if (!reuse)
	{
		free(*arr);
		*arr = NULL;
	}
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

// Finds the environment variable name passed by key and returns its definition
char	*get_env_value(char **envp, const char *key)
{
	int	i;
	int	len;

	i = -1;
	len = ft_strlen(key);
	while (envp[++i])
	{
		if (!ft_strncmp(envp[i], key, len) && envp[i][len] == '=')
		{
			if (envp[i][len] == '=')
				return (envp[i] + len + 1);
			else if (envp[i][len] == '\0')
				return (NULL);
		}
	}
	return (NULL);
}

// Sets/updates an environment variable through their passed definition (value)
void	set_env_value(char ***envp, const char *key, const char *value)
{
	int		i;
	char	**new_envp;

	i = -1;
	while ((*envp)[++i])
	{
		if (is_key_match((*envp)[i], key))
		{
			free((*envp)[i]);
			if (value != NULL)
				(*envp)[i] = ft_strjoin3(key, "=", value);
			else
				(*envp)[i] = ft_strdup(key);
			return ;
		}
	}
	new_envp = realloc_env(*envp, 1);
	if (!new_envp)
		return ;
	if (value != NULL)
		new_envp[i] = ft_strjoin3(key, "=", value);
	else
		new_envp[i] = ft_strdup(key);
	free(*envp);
	*envp = new_envp;
}

// Removes an environemnt variable through its passed name (key)
// NOTE!!! : make sure undefined variable doesn't overwrite existing variable
char	**unset_env_value(char **envp, const char *key, t_shell *shell)
{
	int		i;
	int		j;
	size_t	key_len;
	char	**new_envp;

	key_len = ft_strlen(key);
	new_envp = malloc(sizeof(char *) * ft_arrlen(envp));
	if (!new_envp)
		return (envp);
	i = -1;
	j = 0;
	while (envp[++i])
	{
		if (!ft_strncmp(envp[i], key, key_len)
			&& (envp[i][key_len] == '=' || envp[i][key_len] == '\0'))
		{
			free(envp[i]);
			shell->removed = true;
			continue ;
		}
		new_envp[j++] = envp[i];
	}
	if (!shell->removed)
		return (free(new_envp), envp);
	return (new_envp[j] = NULL, free(envp), new_envp);
}
