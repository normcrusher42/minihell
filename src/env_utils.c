/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 22:05:42 by nanasser          #+#    #+#             */
/*   Updated: 2025/09/04 22:05:42 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

// Finds the environment variable name passed by key and returns its definition.
char	*get_env_value(char **envp, const char *key)
{
	int	i;
	int	len;

	if (!envp || !key)
		return (NULL);
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

// Sets/updates an environment variable through their passed definition (value).
void	set_env_value(char ***envp, const char *key, const char *val, int exist)
{
	int		i;
	char	**new_envp;

	i = -1;
	while ((*envp)[++i])
	{
		if (is_key_match((*envp)[i], key))
		{
			if (exist)
				return ;
			free((*envp)[i]);
			if (val)
				return ((*envp)[i] = ft_strjoin3(key, "=", val), (void)0);
			else
				return ((*envp)[i] = ft_strdup(key), (void)0);
		}
	}
	new_envp = realloc_env(*envp, 1);
	if (!new_envp)
		return ;
	if (val != NULL)
		new_envp[i] = ft_strjoin3(key, "=", val);
	else
		new_envp[i] = ft_strdup(key);
	return (free(*envp), *envp = new_envp, (void)0);
}

// Removes an environemnt variable through its passed name (key).
char	**unset_env_value(char **envp, const char *key, t_shell *shell)
{
	int		i;
	int		j;
	size_t	key_len;
	char	**new_envp;

	key_len = ft_strlen(key);
	new_envp = malloc(sizeof(char *) * (ft_arrlen(envp) + 1));
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
