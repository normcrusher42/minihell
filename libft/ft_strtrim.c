/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 16:37:43 by nanasser          #+#    #+#             */
/*   Updated: 2025/01/18 15:54:40 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		n;
	char	*str;

	if (!s1 || !set)
		return (NULL);
	i = 0;
	str = 0;
	n = ft_strlen(s1);
	if (s1 && set)
	{
		i = 0;
		while (s1[i] && ft_strchr(set, s1[i]))
			i++;
		while (s1[n - 1] && ft_strchr(set, s1[n - 1]) && n > i)
			n--;
		str = malloc(sizeof(char) * (n - i + 1));
		if (!str)
			return (NULL);
		ft_strlcpy(str, &s1[i], (n - i + 1));
	}
	return (str);
}
