/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 18:46:35 by nanasser          #+#    #+#             */
/*   Updated: 2025/04/21 20:04:18 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	cleanup(t_list **list)
{
	t_list	*new_node;
	t_list	*last_node;
	int		i;
	int		n;
	char	*buf;

	n = 0;
	i = 0;
	buf = malloc(BUFFER_SIZE + 1);
	new_node = malloc(sizeof(t_list));
	if (!new_node || !buf)
		return ;
	last_node = ft_lstlast_gnl(*list);
	while (last_node -> content[i] && last_node -> content[i] != '\n')
		i++;
	while (last_node -> content[i])
		buf[n++] = last_node -> content[++i];
	buf[n] = '\0';
	new_node -> content = buf;
	new_node -> next = NULL;
	ft_lstclear_gnl(list, new_node, buf);
}

char	*get_line(t_list *list)
{
	int		len;
	char	*next_str;

	if (!list)
	{
		free(list);
		list = NULL;
		return (NULL);
	}
	len = len_to_nline(list);
	next_str = malloc(len + 1);
	if (!next_str)
		return (NULL);
	str_cpy(list, next_str);
	return (next_str);
}

void	ft_lstjoin(t_list **list, char *buf)
{
	t_list	*new_node;
	t_list	*last_node;

	new_node = malloc(sizeof(t_list));
	last_node = ft_lstlast_gnl(*list);
	if (!new_node)
		return ;
	if (!last_node)
		*list = new_node;
	else
		last_node -> next = new_node;
	new_node -> content = buf;
	new_node -> next = NULL;
}

void	new_lst(t_list **list, int fd)
{
	ssize_t		len;
	char		*buf;

	while (!find_newline(*list))
	{
		buf = malloc(BUFFER_SIZE + 1);
		if (!buf)
			return ;
		len = read(fd, buf, BUFFER_SIZE);
		if (!len)
		{
			free(buf);
			return ;
		}
		buf[len] = '\0';
		ft_lstjoin(list, buf);
	}
}

char	*get_next_line(int fd)
{
	char			*the_line;
	static t_list	*list = NULL;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		if (list)
		{
			free(list -> content);
			list -> content = NULL;
		}
		free(list);
		list = NULL;
		return (NULL);
	}
	new_lst(&list, fd);
	if (!list)
		return (NULL);
	the_line = get_line(list);
	cleanup(&list);
	return (the_line);
}
