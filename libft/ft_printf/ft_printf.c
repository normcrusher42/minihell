/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 13:58:04 by nanasser          #+#    #+#             */
/*   Updated: 2025/02/25 20:17:24 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_putchar(int c)
{
	return (write(1, &c, 1));
}

static int	ft_format(va_list args, const char format)
{
	int	num;

	num = 0;
	if (format == 'c')
		return (num += ft_putchar(va_arg(args, int)));
	else if (format == 's')
		return (num += ft_putstr(va_arg(args, char *)));
	else if (format == 'p')
		return (num += ft_ptr(va_arg(args, uintptr_t)));
	else if (format == 'd' || format == 'i')
		return (num += ft_printnum(va_arg(args, int)));
	else if (format == 'u')
		return (num += ft_uint(va_arg(args, unsigned int)));
	else if (format == 'x' || format == 'X')
		return (num += ft_printhex(va_arg(args, unsigned int), format));
	else if (format == '%')
		return (num += ft_printpercent());
	return (0);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		num;
	int		i;

	num = 0;
	i = 0;
	if (!format)
		return (write(1, "(null)", 6));
	va_start(args, format);
	while (format[i])
	{
		if (format[i] == '%' && format[i + 1] != 0)
		{
			i++;
			if (ft_strchr("cspdiuxX%", format[i]))
				num += ft_format(args, format[i]);
			else
				num += ft_putchar('%') + ft_putchar(format[i]);
		}
		else
			num += ft_putchar(format[i]);
		i++;
	}
	va_end(args);
	return (num);
}

// int	main()
// {
// 	ft_printf("%s - %500% - %s", "lol", "sos");
// 	printf("%s - %500% - %s", "lol", "sos");
// }