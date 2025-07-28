/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 18:19:42 by nanasser          #+#    #+#             */
/*   Updated: 2025/05/27 04:14:04 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "../libft.h"

int	ft_printf(const char *format, ...);
int	ft_putstr(char *str);
int	ft_printnum(int n);
int	ft_printpercent(void);
int	ft_uint(unsigned int n);
int	ft_ptr(uintptr_t ptr);
int	ft_printhex(unsigned int n, const char format);

#endif