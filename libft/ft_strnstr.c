/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 20:43:05 by nanasser          #+#    #+#             */
/*   Updated: 2025/05/27 04:35:43 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	h;
	size_t	n;

	h = 0;
	n = 0;
	if (needle[n] == 0)
	{
		return ((char *) haystack);
	}
	while (haystack[h] && h < len)
	{
		n = 0;
		while (haystack[h + n] == needle[n] && needle[n] && h + n < len)
			n++;
		if (needle[n] == 0)
			return ((char *) &haystack[h]);
		h++;
	}
	return (NULL);
}
