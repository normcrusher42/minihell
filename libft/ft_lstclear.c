/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 19:07:15 by nanasser          #+#    #+#             */
/*   Updated: 2025/01/17 21:28:46 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*move;
	t_list	*temp;

	if (!*lst || !del)
		return ;
	move = *lst;
	while (move)
	{
		temp = move -> next;
		(*del)(move -> content);
		free(move);
		move = temp;
	}
	*lst = NULL;
}
