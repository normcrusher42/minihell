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
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

# define RED "\033[0;31m"
# define BRED "\033[1;31m"
# define RESET "\033[0m"

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
char	**unset_env_value(char **envp, const char *key, t_shell *shell);
int		ft_arrlen(char **arr);
void	free_env(char ***envp);
void	butter_free(t_shell *shell);
void	execute_command(char *cmd, char **env);

#endif