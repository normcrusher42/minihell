/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 14:08:19 by nanasser          #+#    #+#             */
/*   Updated: 2025/05/27 04:29:40 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	whitespaces(const char *str, int *istr)
{
	int	i;
	int	counter;

	i = 0;
	counter = 1;
	while ((str[i] && str[i] >= 9 && str[i] <= 13) || (str[i] == 32))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			counter *= -1;
		i++;
	}
	*istr = i;
	return (counter);
}

int	ft_atoi(const char *str)
{
	int			i;
	int			sign;
	long		result;

	sign = whitespaces(str, &i);
	result = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (result > (LONG_MAX - (str[i] - '0')) / 10)
		{
			if (sign == 1)
				return (-1);
			else if (sign == -1)
				return (0);
		}
		result = (result * 10) + (str[i++] - '0');
	}
	return (result * sign);
}
