/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: team                                           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:58:16 by team              #+#    #+#             */
/*   Updated: 2025/08/10 21:00:00 by team             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*merge_str(char *token, int last_status, int i, char **merge)
{
	char	*pid_str;

	merge[0] = ft_substr(token, 0, i);
	if (token[i + 1] == '$')
	{
		pid_str = ft_itoa(getpid());
		merge[1] = pid_str;
	}
	else if (last_status == -1)
		merge[1] = ft_strdup("");
	else
		merge[1] = ft_itoa(last_status);
	merge[2] = ft_substr(token, i + 2, ft_strlen(token) - (i + 2));
	return (ft_strjoin3(merge[0], merge[1], merge[2]));
}

static char	*env_expander(char *token, char **merge, char **envp, int i)
{
	char		*updated;
	char		**env_vals;
	int			end;

	env_vals = ft_calloc(3, sizeof(char *));
	if (!env_vals)
		return (NULL);
	if (token[i] == '$' && (ft_isalnum(token[i + 1]) || token[i + 1] == '_'))
	{
		end = i + 1;
		while (token[end] && (ft_isalnum(token[end]) || token[end] == '_'))
			end++;
		merge[0] = ft_substr(token, 0, i);
		env_vals[0] = ft_substr(token, i + 1, end - (i + 1));
		merge[1] = ft_substr(token, end, ft_strlen(token) - end);
		env_vals[1] = get_env_value(envp, env_vals[0]);
		if (!env_vals[1])
			env_vals[1] = ft_strdup("");
		else
			env_vals[1] = ft_strdup(env_vals[1]);
		updated = ft_strjoin3(merge[0], env_vals[1], merge[1]);
		return (free_arr(&env_vals, NO), updated);
	}
	return (free_arr(&env_vals, NO), ft_strdup(token));
}

static char	*expand_single_dollar(char *token, char **merge, char **envp, int i)
{
	unsigned char next;

	next = (unsigned char)token[i + 1];
	if (!ft_isalnum(next) && next != '_' && !ft_isspace(next))
		return (merge_str(token, -1, i, merge)); /* $<symbol> → remove both */
	return (env_expander(token, merge, envp, i));
}

/* Work on a local copy: no mutation of the caller's string, no double-free. */
char	*dollar_expander(char *token, int last_status, char **envp)
{
	int		i;
	char	*work;
	char	*updated;
	char	**merge;

	if (!token)
		return (NULL);
	work = ft_strdup(token);
	merge = ft_calloc(4, sizeof(char *));
	if (!work || !merge)
		return (free(work), free(merge), NULL);
	i = -1;
	while (work[++i])
	{
		free_arr(&merge, YES);
		if (work[i] == '$' && (work[i + 1] == '$' || work[i + 1] == '?'))
			updated = merge_str(work, last_status, i, merge);
		else if (work[i] == '$' && work[i + 1] && work[i + 1] != '$')
			updated = expand_single_dollar(work, merge, envp, i);
		else
			continue ;
		free(work);
		work = updated;
		i = -1; /* restart to catch further $ after structural change */
	}
	free_arr(&merge, NO);
	return (work);
}

char	**expand_token(t_token *token, char **envp, int last_status)
{
	int		i;
	int		n;
	char	**result;

	if (!token || !token->tokens)
		return (NULL);
	n = ft_arrlen(token->tokens);
	result = malloc(sizeof(char *) * (n + 1));
	if (!result)
		return (NULL);
	i = -1;
	while (token->tokens[++i])
	{
		if (ft_strchr(token->tokens[i], '$') && token->quote[i] != QTE_SINGLE)
			result[i] = dollar_expander(token->tokens[i], last_status, envp);
		else
			result[i] = ft_strdup(token->tokens[i]);
		if (!result[i])
			return (free_arr(&result, NO), NULL);
	}
	result[i] = NULL;
	free_arr(&token->tokens, NO);
	return (result);
}
