/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_str.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:06:38 by nanasser          #+#    #+#             */
/*   Updated: 2025/02/02 18:44:29 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putstr(char *str)
{
	int	i;

	i = 0;
	if (str == NULL)
	{
		write(1, "(null)", 6);
		return (6);
	}
	while (str[i])
	{
		write(1, &str[i], 1);
		i++;
	}
	return (i);
}

int	ft_printnum(int n)
{
	int		num;
	char	*str;

	num = 0;
	str = ft_itoa(n);
	num = ft_putstr(str);
	free(str);
	return (num);
}

int	ft_printpercent(void)
{
	write(1, "%", 1);
	return (1);
}
