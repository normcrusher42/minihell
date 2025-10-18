/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 04:41:54 by nanasser          #+#    #+#             */
/*   Updated: 2025/09/29 04:41:54 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// A simple remake of 'pwd', prints the current working directory.
int	ft_pwd(t_shell *sh)
{
	char	buf[1024];
	char	*pwd;

	if (getcwd(buf, sizeof(buf)))
		return (ft_putendl_fd(buf, 1), 0);
	pwd = get_env_value(sh->envp, "PWD");
	if (!pwd)
		ft_putendl_fd("", 1);
	else
		ft_putendl_fd(pwd, 1);
	return (1);
}
