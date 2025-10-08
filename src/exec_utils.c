/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 20:04:32 by nanasser          #+#    #+#             */
/*   Updated: 2025/10/08 20:51:12 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// strncmp except it actually compares full string lengths cs it aint in libft??
int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while ((s1[i] || s2[i]))
	{
		if (s1[i] != s2[i])
			return ((unsigned char) s1[i] - (unsigned char) s2[i]);
		i++;
	}
	return (0);
}

void	try_direct_exec(char **av, char ***env, t_shell *sh)
{
	struct stat	st;

	(void)sh;
	execve(av[0], av, *env);
	ft_putstr_fd("miniOdy: ", 2);
	if (stat(av[0], &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_putstr_fd(av[0], 2);
		ft_putendl_fd(": Is a directory", 2);
		call_janitor(sh);
		exit(126);
	}
	perror(av[0]);
	call_janitor(sh);
	if (errno == EACCES)
		exit(126);
	exit(127);
}
