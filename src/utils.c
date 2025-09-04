/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: team                                           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 21:25:21 by team              #+#    #+#             */
/*   Updated: 2025/08/10 21:00:00 by team             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_key_match(const char *env, const char *key)
{
	size_t	len;

	len = ft_strlen(key);
	if (ft_strncmp(env, key, len) != 0)
		return (0);
	if (env[len] == '=' || env[len] == '\0')
		return (1);
	return (0);
}

// Self-explanatory.
// Merges 3 strings together. Mainly used for the expander and env creation
char	*ft_strjoin3(const char *key, const char *input, const char *value)
{
	char	*temp;
	char	*new_str;

	temp = ft_strjoin(key, input);
	new_str = ft_strjoin(temp, value);
	free(temp);
	return (new_str);
}

int	ft_isspace(int c)
{
	return ((c >= 9 && c <= 13) || c == ' ');
}

int	ft_arrlen(char **arr)
{
	int	i;

	if (!arr)
		return (0);
	i = 0;
	while (arr[i])
		i++;
	return (i);
}

int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i] || !ft_isdigit((unsigned char)str[i]))
		return (0);
	while (str[i])
		if (!ft_isdigit((unsigned char)str[i++]))
			return (0);
	return (1);
}
