/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_unsigned.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:06:39 by nanasser          #+#    #+#             */
/*   Updated: 2025/02/25 18:35:59 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_intlen(unsigned int n)
{
	unsigned int	count;

	count = 0;
	while (n != 0)
	{
		n = n / 10;
		count++;
	}
	return (count);
}

static void	ft_uitoa(char *str, unsigned int n, int size)
{
	while (size > 0)
	{
		str[size - 1] = (n % 10 + '0');
		n = n / 10;
		size--;
	}
}

int	ft_uint(unsigned int n)
{
	int		num;
	int		size;
	char	*str;

	num = 0;
	if (n == 0)
		return (write(1, "0", 1));
	size = ft_intlen(n);
	str = malloc(sizeof(char) * (size + 1));
	if (!str)
		return (0);
	ft_uitoa(str, n, size);
	str[size] = '\0';
	num += ft_putstr(str);
	free(str);
	str = NULL;
	return (num);
}
