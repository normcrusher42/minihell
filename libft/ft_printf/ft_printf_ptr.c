/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_ptr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:06:37 by nanasser          #+#    #+#             */
/*   Updated: 2025/02/04 18:16:05 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_ptrlen(uintptr_t n)
{
	unsigned int	count;

	count = 0;
	while (n != 0)
	{
		n = n / 16;
		count++;
	}
	return (count);
}

static void	ft_putptr(uintptr_t n)
{
	if (n >= 16)
	{
		ft_putptr(n / 16);
		ft_putptr(n % 16);
	}
	else
	{
		if (n <= 9)
			ft_putchar_fd((n + '0'), 1);
		else
			ft_putchar_fd((n - 10 + 'a'), 1);
	}
}

int	ft_ptr(uintptr_t ptr)
{
	int	num;

	num = 0;
	num += write(1, "0x", 2);
	if (ptr == 0)
		return (num += write(1, "0", 1));
	else
	{
		ft_putptr(ptr);
		num += ft_ptrlen(ptr);
	}
	return (num);
}
