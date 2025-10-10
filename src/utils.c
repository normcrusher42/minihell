/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 15:57:22 by nanasser          #+#    #+#             */
/*   Updated: 2025/10/10 15:57:22 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Compares strings and checks if they're matching and checks their last value.
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
// Merges 3 strings together. Mainly used for the expander and env creation.
char	*ft_strjoin3(const char *key, const char *input, const char *value)
{
	char	*temp;
	char	*new_str;

	temp = ft_strjoin(key, input);
	new_str = ft_strjoin(temp, value);
	free(temp);
	return (new_str);
}

// Self-explanatory.
// Checks if the character is a whitespace character.
int	ft_isspace(int c)
{
	return ((c >= 9 && c <= 13) || c == ' ');
}

// Self-explanatory.
// But Nasser's dumbass forgot we could do that with strlen 🗿
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

// Checks if the passed string is numeric (atoi hacks basically).
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
