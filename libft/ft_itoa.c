/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 14:54:10 by nanasser          #+#    #+#             */
/*   Updated: 2025/05/27 04:32:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_intlen(int n)
{
	unsigned int	count;

	count = 0;
	if (n <= 0)
		count++;
	while (n != 0)
	{
		n = n / 10;
		count++;
	}
	return (count);
}

static void	strlcpypro(char *str, int n, int start, int size)
{
	while (size > start)
	{
		str[size - 1] = n % 10 + '0';
		n = n / 10;
		size--;
	}
}

char	*ft_itoa(int n)
{
	int		start;
	int		size;
	char	*str;

	start = 0;
	size = ft_intlen(n);
	str = malloc(sizeof(char) * size + 1);
	if (!str)
		return (NULL);
	if (n == -2147483648)
	{
		str[0] = '-';
		str[1] = '2';
		n = 147483648;
		start = 2;
	}
	if (n < 0)
	{
		str[0] = '-';
		n = -n;
		start = 1;
	}
	strlcpypro(str, n, start, size);
	str[size] = '\0';
	return (str);
}
