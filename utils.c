/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 21:25:21 by nanasser          #+#    #+#             */
/*   Updated: 2025/07/31 21:26:18 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_atoll(const char *str)
{
	int			i;
	int			sign;
	long		result;

	sign = 1;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	result = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (result > (LLONG_MAX / 10))
			return (-1);
		result = (result * 10) + (str[i++] - '0');
	}
	return (result * sign);
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
	if ((c >= 9 && c <= 13) || c == ' ')
		return (1);
	return (0);
}

int	ft_arrlen(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}

int	is_numeric(const char *str)
{
	int i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i] || !ft_isdigit((unsigned char)str[i]))
		return (0);
	while (str[i])
		if (!ft_isdigit((unsigned char)str[i++]))
			return (0);
	return (1);
}
