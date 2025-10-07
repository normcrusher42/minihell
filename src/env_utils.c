/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 04:40:45 by nanasser          #+#    #+#             */
/*   Updated: 2025/09/29 04:40:45 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	replace_env_value(char **slot, const char *key, const char *val)
{
	free(*slot);
	if (val)
		*slot = ft_strjoin3(key, "=", val);
	else
		*slot = ft_strdup(key);
}

// Sets/updates an environment variable through their passed definition (value).
void	set_env_value(char ***envp, const char *key, const char *val, int exist)
{
	t_envctx	e;

	e.i = -1;
	e.key = (char *)key;
	e.val = (char *)val;
	e.exist = exist;
	while ((*envp)[++e.i])
	{
		if (is_key_match((*envp)[e.i], e.key))
		{
			if (e.exist)
				return ;
			replace_env_value(&(*envp)[e.i], e.key, e.val);
			return ;
		}
	}
	e.new_envp = realloc_env(*envp, 1);
	if (!e.new_envp)
		return ;
	if (e.val)
		e.new_envp[e.i] = ft_strjoin3(e.key, "=", e.val);
	else
		e.new_envp[e.i] = ft_strdup(e.key);
	free(*envp);
	*envp = e.new_envp;
}

// Helper to check if the current env entry is the target variable to remove.
static bool	handle_target_var(char *entry, const char *key, t_envctx *u)
{
	if (!ft_strncmp(entry, key, u->key_len)
		&& (entry[u->key_len] == '=' || entry[u->key_len] == '\0'))
	{
		free(entry);
		u->removed = true;
		return (true);
	}
	return (false);
}

// Removes an environemnt variable through its passed name (key).
char	**unset_env_value(char **envp, const char *key)
{
	t_envctx	u;

	u = (t_envctx){0};
	u.i = -1;
	u.key_len = ft_strlen(key);
	u.new_envp = malloc(sizeof(char *) * (ft_arrlen(envp) + 1));
	if (!u.new_envp)
		return (envp);
	while (envp[++u.i])
	{
		if (handle_target_var(envp[u.i], key, &u))
			continue ;
		u.new_envp[u.j++] = envp[u.i];
	}
	if (!u.removed)
	{
		free(u.new_envp);
		return (envp);
	}
	u.new_envp[u.j] = NULL;
	free(envp);
	return (u.new_envp);
}
