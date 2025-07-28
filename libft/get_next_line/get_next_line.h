/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 18:46:33 by nanasser          #+#    #+#             */
/*   Updated: 2025/05/27 04:28:32 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

// Buffersize definition
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42

# endif

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <stdio.h>

// Singly Linked list struct
typedef struct s_list
{
	char			*content;
	struct s_list	*next;
}	t_list;

char	*get_next_line(int fd);
t_list	*ft_lstlast_gnl(t_list *lst);
void	ft_lstclear_gnl(t_list **lst, t_list *new_node, char *buf);
int		find_newline(t_list *list);
void	new_lst(t_list **list, int fd);
void	ft_lstjoin(t_list **list, char *buf);
int		len_to_nline(t_list *list);
char	*get_line(t_list *list);
void	str_cpy(t_list *list, char *str);
void	cleanup(t_list **list);

#endif

/* FD access modes:
O_RDONLY	Open for reading only
O_WRONLY	Open for writing only
O_RDWR		Open for reading and writing
O_CREAT		Create file if it does not exist
O_TRUNC		Truncate file to 0 length if it exists
O_APPEND	Append to the end of the file 
*/
