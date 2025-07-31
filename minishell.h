/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:37:55 by nanasser          #+#    #+#             */
/*   Updated: 2025/07/31 18:50:16 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include "libft/libft.h"
# include "libft/ft_printf/ft_printf.h"
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_shell
{
	char	*input;
	bool	last_exit_status;
	char	**envp;
}	t_shell;

int		main(int argc, char **argv, char **envp);
void	butter_free(t_shell *shell);

#endif