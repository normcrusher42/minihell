/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:37:55 by nanasser          #+#    #+#             */
/*   Updated: 2025/07/31 21:31:35 by nanasser         ###   ########.fr       */
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
	bool	removed;
	bool	last_exit_status;
	char	**envp;
}	t_shell;

int		main(int argc, char **argv, char **envp);
char	*get_env_value(char **envp, const char *key);
void	set_env_value(char ***envp, const char *key, const char *value);
void	unset_env_value(char ***envp, const char *key, t_shell *shell);
int		ft_arrlen(char **arr);
void	free_env(char ***envp);
void	butter_free(t_shell *shell);

#endif