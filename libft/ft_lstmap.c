/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 20:50:57 by nanasser          #+#    #+#             */
/*   Updated: 2025/01/17 21:28:31 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*list;
	t_list	*newnode;

	if (!lst || !f)
		return (NULL);
	list = NULL;
	while (lst)
	{
		newnode = ft_lstnew(f(lst -> content));
		if (!newnode)
		{
			ft_lstclear(&list, *(del));
			return (NULL);
		}
		ft_lstadd_back(&list, newnode);
		lst = lst -> next;
	}
	return (list);
}
