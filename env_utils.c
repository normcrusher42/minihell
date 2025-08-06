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

void	free_arr(char ***envp)
{
	int	i;

	i = -1;
	while ((*envp)[++i])
	{
		if ((*envp)[i])
		{
			free((*envp)[i]);
			(*envp)[i] = NULL;
		}
	}
	if (*envp)
		free(*envp);
	*envp = NULL;
}

char	*ft_strjoin3(const char *key, const char *input, const char *value)
{
	char	*temp;
	char	*new_str;

	temp = ft_strjoin(key, input);
	new_str = ft_strjoin(temp, value);
	free(temp);
	return (new_str);
}

char	*get_env_value(char **envp, const char *key)
{
	int	i;
	int	len;

	i = -1;
	len = ft_strlen(key);
	while (envp[++i])
		if (!ft_strncmp(envp[i], key, len) && envp[i][len] == '=')
			return (envp[i] + len + 1);
	return (NULL);
}

void	set_env_value(char ***envp, const char *key, const char *value)
{
	int		i;
	int		len;
	char	**new_envp;

	i = -1;
	len = ft_strlen(key);
	while ((*envp)[++i])
	{
		if (!ft_strncmp((*envp)[i], key, len) && (*envp)[i][len] == '=')
		{
			free((*envp)[i]);
			(*envp)[i] = ft_strjoin3(key, "=", value);
			return ;
		}
	}
	new_envp = malloc(sizeof(char *) * (i + 2));
	if (!new_envp)
		return ;
	i = -1;
	while ((*envp)[++i])
		new_envp[i] = (*envp)[i];
	new_envp[i++] = ft_strjoin3(key, "=", value);
	new_envp[i] = NULL;
	free(*envp);
	*envp = new_envp;
}

char	**unset_env_value(char **envp, const char *key, t_shell *shell)
{
	int		i;
	int		j;
	int		len;
	char	**new_envp;

	len = ft_strlen(key);
	i = ft_arrlen(envp);
	new_envp = malloc(sizeof(char *) * i);
	if (!new_envp)
		return (envp);
	i = -1;
	j = 0;
	while (envp[++i])
	{
		if (!ft_strncmp(envp[i], key, len) && envp[i][len] == '=')
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
