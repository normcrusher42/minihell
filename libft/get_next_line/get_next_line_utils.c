/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 18:46:36 by nanasser          #+#    #+#             */
/*   Updated: 2025/04/21 20:03:34 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_list	*ft_lstlast_gnl(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst -> next != NULL)
	{
		lst = lst -> next;
	}
	return (lst);
}

void	ft_lstclear_gnl(t_list **lst, t_list *new_node, char *buf)
{
	t_list	*move;
	t_list	*temp;

	if (!*lst)
		return ;
	move = *lst;
	while (move)
	{
		temp = move -> next;
		free(move -> content);
		move -> content = NULL;
		free(move);
		move = temp;
	}
	*lst = NULL;
	if (new_node -> content[0])
		*lst = new_node;
	else
	{
		free(buf);
		buf = NULL;
		free(new_node);
		new_node = NULL;
	}
}

int	find_newline(t_list *list)
{
	int	i;

	if (!list)
		return (0);
	while (list)
	{
		i = 0;
		while (list -> content[i] && i < BUFFER_SIZE)
		{
			if (list -> content[i] == '\n')
				return (1);
			i++;
		}
		list = list -> next;
	}
	return (0);
}

int	len_to_nline(t_list *list)
{
	int	i;
	int	len;

	if (!list)
		return (0);
	len = 0;
	while (list)
	{
		i = 0;
		while (list -> content[i])
		{
			if (list -> content[i] == '\n')
				return (++len);
			i++;
			len++;
		}
		list = list -> next;
	}
	return (len);
}

void	str_cpy(t_list *list, char *str)
{
	int	i;
	int	n;

	if (!list)
		return ;
	n = 0;
	while (list)
	{
		i = 0;
		while (list -> content[i])
		{
			if (list -> content[i] == '\n')
			{
				str[n] = '\n';
				str[++n] = '\0';
				return ;
			}
			str[n++] = list -> content[i++];
		}
		list = list -> next;
	}
	str[n] = '\0';
}
