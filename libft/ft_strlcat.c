/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 17:59:14 by nanasser          #+#    #+#             */
/*   Updated: 2025/05/27 04:35:21 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, char *src, size_t dstsize)
{
	size_t	i;
	size_t	n;
	size_t	dlen;
	size_t	slen;

	i = ft_strlen(dst);
	n = 0;
	dlen = i;
	slen = ft_strlen(src);
	if (dstsize == 0 || dlen >= dstsize)
		return (slen + dstsize);
	while (src[n] && n < dstsize - dlen - 1)
	{
		dst[i] = src[n];
		i++;
		n++;
	}
	dst[i] = '\0';
	return (slen + dlen);
}
