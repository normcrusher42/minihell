/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 17:40:37 by nanasser          #+#    #+#             */
/*   Updated: 2025/05/27 04:32:46 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *str, int c, size_t n)
{
	size_t			i;
	unsigned char	*str2;

	i = 0;
	str2 = (unsigned char *) str;
	while (i < n)
	{
		if (str2[i] == (unsigned char)c)
			return ((void *)&str2[i]);
		i++;
	}
	return (NULL);
}
